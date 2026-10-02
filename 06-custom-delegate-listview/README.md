# 06 – Custom delegate in a QListView

A contact list where every row is drawn as a card: a colored avatar with initials and a presence badge, the name,
a detail line (title · email) and a favorite star. Clicking the star toggles the favorite flag.

## Model and delegate split

- `ContactListModel` only provides **data**, through custom roles (`NameRole`, `TitleRole`, `EmailRole`,
  `PresenceRole`, `FavoriteRole`).
- `ContactDelegate` only decides **how it looks and how it reacts**. It reads everything through
  `index.data(role)` and never touches the model's internals.

So the same model could be shown in a plain `QListView`, a QML `ListView` or a test.

## The three delegate functions

| Function | Purpose |
|---|---|
| `sizeHint()` | Height of a row (fixed 68 px here, so `setUniformItemSizes(true)` is safe and fast). |
| `paint()` | Draws the item with `QPainter` into `option.rect`. |
| `editorEvent()` | Receives mouse and key events for the item. Used for the star "button". |

### Painting

`paint()` is called often, so it only computes rectangles and draws:

1. Read the state from `option.state`: `State_Selected`, `State_MouseOver`.
2. Pick colors from `option.palette` (`Highlight`, `Base`, `Text`, …). The delegate then follows light and
   dark themes automatically.
3. Draw the card, avatar, texts and star. Text is shortened with `QFontMetrics::elidedText()` so it never
   overflows.
4. `painter->save()` / `restore()` around everything, because the painter is shared with the view.

Hover only works if the view tracks the mouse: `view.setMouseTracking(true)`.

### Interaction without widgets

The star is not a widget, just a rectangle. `editorEvent()` checks whether a click landed inside it and writes
the new value back through `model->setData(index, value, FavoriteRole)`. The model emits `dataChanged()` and
the view repaints that row. The same helper (`starRect()`) is used for painting and hit-testing, so they always
agree.

## Why not `setIndexWidget()`?

Putting real widgets into every row seems easier but does not scale. Each row would have its own widget with
its own events and layout. A delegate paints thousands of rows using a single object, and only the visible rows
are painted.

## Files

```
src/
  Contact.h                  data
  ContactListModel.h/.cpp    list model with custom roles
  ContactDelegate.h/.cpp     painting + click handling
  main.cpp                   sample data and the QListView
```
