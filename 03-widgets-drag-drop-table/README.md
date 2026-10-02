# 03 – Drag & drop between tables with a capacity limit (Widgets)

Two `QTableView`s: **Available items** and **Selected items**. Rows are moved between them by drag & drop.
The selected table accepts at most *N* items (default 10). *N* can be changed at runtime with the spin box.

## Qt's model/view drag & drop protocol

The views do the mouse handling. The **model** decides what can be dragged, how it is serialized and whether a
drop is accepted. For a move, these calls happen in this order:

| Step | Who | Model function |
|---|---|---|
| Can this row be dragged? | source view | `flags()` returns `Qt::ItemIsDragEnabled` |
| Which actions are allowed? | source view | `supportedDragActions()` → `Qt::MoveAction` |
| Pack the dragged rows | source view | `mimeData(indexes)` |
| Hovering: is the drop allowed here? | target view | `mimeTypes()`, `canDropMimeData(...)` |
| Drop | target view | `dropMimeData(...)` inserts the rows |
| Finish the move | source view | `removeRows(...)` removes the originals |

So a move is *insert in the target, then remove from the source*. Neither model knows about the other.

## Enforcing the limit

The limit lives in the model, so the view cannot get around it:

```cpp
bool ItemTableModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, ...) const
{
    if (action != Qt::MoveAction || !data->hasFormat(itemMimeType))
        return false;
    if (!m_capacity)
        return true;
    return m_items.size() + decode(data).size() <= *m_capacity;
}
```

`QAbstractItemView` calls `canDropMimeData()` while hovering, so the cursor shows a "forbidden" sign as soon as
the table is full. `dropMimeData()` checks again, because a model must never rely on the view.

The capacity is a `std::optional<int>`: the *Available* table has no limit, the *Selected* table has one.

### Lowering the limit below the current count

If 10 items are selected and the limit is lowered to 5, nothing is removed. The table simply counts as full and
accepts no more drops until items are dragged out. This avoids deleting the user's work without asking.

## MIME data

Rows are serialized with `QDataStream` into a custom MIME type `application/x-qtexamples-item`
(see the stream operators in `Item.h`). Using a custom type means other applications, or other tables in your
app that do not understand items, will not accept the drop.

## ViewModel

`SelectionViewModel` owns both models and exposes `capacity`, `summary` ("7 / 10 selected") and `isFull`.
`SelectionDialog` only binds widgets to it, like in example [01](../01-widgets-mvvm-dialog).

## Files

```
src/
  Item.h                    data + QDataStream operators
  ItemTableModel.h/.cpp     table model with DnD + capacity
  SelectionViewModel.h/.cpp owns both models, exposes capacity/summary
  SelectionDialog.h/.cpp    the two tables, spin box, buttons
  main.cpp                  prints the selection on OK
```
