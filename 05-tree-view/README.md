# 05 – TreeView with a custom QAbstractItemModel

Shows a building project hierarchy *Project › Building › Floor › Element* in a `QTreeView`, with three columns:
**Name**, **Type** and **Elements** (the number of elements below a node). You can rename nodes (double-click or
F2), add children, remove nodes and filter by name. Sorting is available by clicking a header.

## The data structure

`TreeNode` is plain C++. Each node owns its children through `std::vector<std::unique_ptr<TreeNode>>` and keeps a
raw pointer to its parent. An invisible root node (`NodeType::Root`) holds the projects.

## How the model gets its data

`ProjectTreeModel` does **not** build a structure of its own. It neither copies nor converts the tree. It keeps
the `TreeNode` tree it receives and translates the view's questions ("how many children does this have?",
"what is row 2 under this?") into lookups on that tree.

### Where the tree comes from

`MainWindow`'s constructor creates the tree and hands it to the model:

```cpp
m_model = new ProjectTreeModel(createSampleProjects(), this);
```

- `createSampleProjects()` (`SampleProject.cpp`) builds the plain C++ tree with `TreeNode::addChild()`.
- The constructor only takes ownership:

  ```cpp
  ProjectTreeModel::ProjectTreeModel(std::unique_ptr<TreeNode> root, QObject *parent)
      : QAbstractItemModel(parent), m_root(std::move(root)) {}
  ```

  At this point no `QModelIndex` exists yet.

### What triggers it

```cpp
m_proxy->setSourceModel(m_model);   // the proxy starts asking the model
m_tree->setModel(m_proxy);          // the view starts asking the proxy
m_tree->expandToDepth(1);           // the view asks for the children of the top two levels
```

Once the view has a model, it calls `rowCount()`, `index()` and `data()` whenever it needs to lay out or paint.
That happens on the first show, when you scroll or expand a node, and when the proxy filters.
A collapsed branch is never asked about until you open it. The model is **pulled**, not pushed.

### Walkthrough: the first show

1. `rowCount(QModelIndex())`: the root has 2 children (the projects).
2. `index(0, 0, {})`: an index carrying the *Residential Lakeside* node. `data()` returns its name.
3. `expandToDepth(1)`: `rowCount(projectIndex)`, then `index(0, 0, projectIndex)` for *House A*, and so on.
4. Floors and elements are only requested when you expand a building.

## Implementing QAbstractItemModel

A tree model has to answer five questions:

| Function | Question |
|---|---|
| `index(row, column, parent)` | What is the index of the *row*-th child of *parent*? |
| `parent(index)` | Which index is the parent of *index*? |
| `rowCount(parent)` | How many children does *parent* have? |
| `columnCount(parent)` | How many columns? |
| `data(index, role)` | What should be shown for *index*? |

The trick is `internalPointer()`. Each `QModelIndex` carries a `TreeNode*`:

```cpp
QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return {};
    return createIndex(row, column, nodeFor(parent)->child(row));
}

TreeNode *ProjectTreeModel::nodeFor(const QModelIndex &index) const
{
    return index.isValid() ? static_cast<TreeNode *>(index.internalPointer()) : m_root.get();
}
```

`nodeFor()` turns an index back into its node, so every other function is a short lookup:

- `rowCount(parent)` returns `nodeFor(parent)->childCount()`.
- `data(index)` returns the name, the type or `elementCount()` of `nodeFor(index)`, depending on the column.
- `parent(index)` goes the other way, using the node's parent pointer:

  ```cpp
  TreeNode *parentNode = nodeFor(index)->parent();
  if (!parentNode || parentNode == m_root.get())
      return {};                                   // top level → invalid index
  return createIndex(parentNode->row(), 0, parentNode);
  ```

Rules to remember:

- An invalid `QModelIndex` stands for the invisible root.
- `parent()` of a top-level item returns an invalid index, never an index for the root node.
- A parent index always uses **column 0**.
- Only column 0 has children: `rowCount()` returns 0 when `parent.column() > 0`.

## Changing the structure

The model only modifies the tree when it is edited (`insertRows()`, `removeRows()`, `setData()`).
Each change must be wrapped in begin/end calls (or followed by `dataChanged()`). They tell the proxy and the
view to ask again for the affected part:

```cpp
beginInsertRows(parent, row, row + count - 1);
parentNode->insertChild(...);
endInsertRows();
```

Adding or removing an element changes the **Elements** column of every ancestor.
`notifyElementCountChanged()` walks up the tree and emits `dataChanged()` for those cells.

## Filtering and sorting

A `QSortFilterProxyModel` sits between the model and the view:

```cpp
m_proxy->setRecursiveFilteringEnabled(true);
```

With recursive filtering, a node is shown if it **or any of its descendants** matches. A search for "rafter"
therefore keeps *Project › Building › Roof* visible above the matching rafters.

The window works with proxy indices, so `MainWindow` maps them with `mapToSource()` / `mapFromSource()`
before talking to `ProjectTreeModel`.

## Testing

Wrap the model in `QAbstractItemModelTester` (QtTest) during development. It checks all the rules above
automatically every time the model changes.

## Files

```
src/
  TreeNode.h/.cpp          tree data structure
  ProjectTreeModel.h/.cpp  QAbstractItemModel on top of TreeNode
  SampleProject.h/.cpp     sample data
  MainWindow.h/.cpp        tree view, toolbar, filter
  main.cpp
```
