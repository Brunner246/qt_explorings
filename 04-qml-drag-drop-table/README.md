# 04 – Drag & drop between tables with a capacity limit (QML)

The same behavior as [03](../03-widgets-drag-drop-table), with the View in QML inside a `QQuickWidget`.
A row can be dragged from one `TableView` to the other. The *Selected* table accepts at most *N* items and
turns red while a drag hovers over it when it is full.

## How QML drag & drop differs from Widgets

| | Widgets (03) | QML (04) |
|---|---|---|
| Who starts the drag | `QAbstractItemView` | a `DragHandler` in the delegate |
| Payload | `QMimeData` built by the model | a QML object (`DragGhost`) carrying `sourceModel` + `sourceRow` |
| Who accepts the drop | `canDropMimeData()` / `dropMimeData()` | a `DropArea` calling into C++ |
| Who moves the data | the view calls `removeRows()` on the source | one C++ call: `sourceModel.moveRowTo(row, target)` |

QML has no built-in model-level DnD protocol. So the data change goes through one explicit
`Q_INVOKABLE`. This keeps the rule ("the target must not be full") in C++, where it is testable:

```cpp
bool ItemTableModel::moveRowTo(int row, ItemTableModel *target)
{
    if (!target || target == this || target->isFull() || row < 0 || row >= m_items.size())
        return false;
    target->append(m_items.at(row));
    removeAt(row);
    return true;
}
```

## Where a drop into a full table is rejected

The QML does not reject the item. The C++ model does, and the QML only reacts to the result.

**1. The rule lives in C++.** `ItemTableModel::moveRowTo()` (above) returns `false` when `target->isFull()`,
where `isFull()` means `count >= capacity`. Nothing is moved.

**2. The drop handler forwards the decision.** `ItemTable.qml`:

```qml
onDropped: drop => {
    const ghost = drop.source as DragGhost
    if (ghost && ghost.sourceModel.moveRowTo(ghost.sourceRow, control.tableModel))
        drop.accept()
}
```

If `moveRowTo()` returns `false`, `drop.accept()` is never called. The drop is ignored and the item stays in
the source table.

**3. The rest of the QML only shows the state.** The `accepting` / `rejecting` properties of `ItemTable.qml` read
`tableModel.isFull` only to color the border. `onEntered` rejects only drags that come from the same table:

```qml
onEntered: drag => drag.accepted = (drag.source as DragGhost)?.sourceModel !== control.tableModel
```

### Why not reject a full table already in `onEntered`?

If `onEntered` rejects a drag, the `DropArea` never reports `containsDrag`, so the red "full" border could not be
shown. Accepting the enter and refusing on drop gives the user visible feedback while hovering. The cost is
that there is no "forbidden" cursor, unlike the Widgets version, where `canDropMimeData()` provides one.

To block the drag already on enter, extend the condition. You then lose the red hover feedback:

```qml
onEntered: drag => drag.accepted = (drag.source as DragGhost)?.sourceModel !== control.tableModel
                                   && !control.tableModel.isFull
```

Either way, `moveRowTo()` stays the final guard, so the limit holds no matter what the QML does.

## The pieces

- **`DragGhost.qml`**: a single floating rectangle at the root of the scene. While dragging it follows the
  pointer, shows the item name and is the `Drag.source`. Keeping one ghost at the root avoids the problem of a
  delegate being clipped by its `TableView` or being destroyed while it is still being dragged.
- **`ItemTable.qml`**: a header, a `TableView` and a `DropArea`. Each cell delegate has a `DragHandler` with
  `target: null`. It does not move the cell itself; it only tells the ghost to start, move and finish.
  The `DropArea` calls `moveRowTo()` on drop and colors the border:
  - highlight color: the drop will be accepted
  - red: the table is full
- **`SelectionView.qml`**: two `ItemTable`s, the capacity `SpinBox` and the summary label.

## C++ side

- `ItemTableModel` adds `roleNames()` (`display` per column, `name` for the whole row) and the properties
  `count` and `isFull` for QML.
- `SelectionViewModel` exposes both models as `CONSTANT` properties plus `capacity`, `summary` and `isFull`.
- `SelectionDialog` injects the ViewModel through `createWithInitialProperties()`, like in
  [02](../02-qml-mvvm-dialog#injecting-the-instance).

## Notes

- `pragma ComponentBehavior: Bound` in `ItemTable.qml` lets the delegate safely use ids of the surrounding
  component (`control`). Without it, `qmllint` warns about unqualified access.
- Dropping on the table the drag came from is ignored (`DropArea.onEntered` rejects it). `moveRowTo()` also
  returns `false` for `target == this`.

## Files

```
src/
  Item.h, ItemTableModel.h/.cpp, SelectionViewModel.h/.cpp
  SelectionDialog.h/.cpp, main.cpp
qml/
  SelectionView.qml   root of the view
  ItemTable.qml       one table with drop handling
  DragGhost.qml       the floating drag payload
```
