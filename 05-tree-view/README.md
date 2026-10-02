# 05 – TreeView with a custom QAbstractItemModel

Shows a building project hierarchy *Project › Building › Floor › Element* in a `QTreeView`, with three columns:
**Name**, **Type** and **Elements** (the number of elements below a node). You can rename nodes (double-click or
F2), add children, remove nodes and filter by name. Sorting is available by clicking a header.

## The data structure

`TreeNode` is plain C++. Each node owns its children through `std::vector<std::unique_ptr<TreeNode>>` and keeps a
raw pointer to its parent. An invisible root node (`NodeType::Root`) holds the projects.

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

Rules to remember:

- An invalid `QModelIndex` stands for the invisible root.
- `parent()` of a top-level item returns an invalid index, never an index for the root node.
- A parent index always uses **column 0**.
- Only column 0 has children: `rowCount()` returns 0 when `parent.column() > 0`.

## Changing the structure

Changes must be wrapped in begin/end calls so that views and proxies can update:

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
