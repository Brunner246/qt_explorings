# 01 – MVVM with a classic QDialog

A small main window shows a person. **Edit…** opens a dialog in which name, email, age and role can be changed.
OK is only enabled while the input is valid; **Reset** restores the original values.

## The idea: Model – View – ViewModel

```
┌──────────┐   load() / toPerson()   ┌─────────────────┐   signals / setters   ┌──────────────┐
│  Person  │ ──────────────────────▶ │ PersonViewModel │ ◀──────────────────▶ │ PersonDialog │
│ (Model)  │ ◀────────────────────── │   (ViewModel)   │                      │    (View)    │
└──────────┘                         └─────────────────┘                      └──────────────┘
 plain data                           state + validation                       widgets only
```

| Part | File | Responsibility |
|---|---|---|
| Model | `Person.h` | Plain data. Knows nothing about Qt objects or UI. |
| ViewModel | `PersonViewModel.h/.cpp` | Holds the *editing state*, validates it and exposes it as `Q_PROPERTY`s with change signals. |
| View | `PersonDialog.h/.cpp` | Creates widgets and *binds* them to the ViewModel. Contains no business logic. |

### Data flow

1. `MainWindow::editPerson()` creates a `PersonViewModel` and loads the current `Person` into it.
2. The dialog binds both directions:
   - **View → ViewModel**: `QLineEdit::textEdited`, `QSpinBox::valueChanged`, … call the ViewModel setters.
   - **ViewModel → View**: every `…Changed` signal triggers `updateFromViewModel()`, which copies the state into
     the widgets.
3. Each setter calls `validate()`, which updates `isValid` and `validationMessage`.
   The View reacts by enabling or disabling the OK button.
4. Only if the dialog is accepted does `MainWindow` call `toPerson()` and replace its model.
   Cancel discards the ViewModel, so the model stays untouched.

### Why no endless signal loop?

Every setter returns early if the value did not change. `textEdited` (unlike `textChanged`) only fires for
user input, and `updateFromViewModel()` only calls `setText()` when the text is actually different, so the
cursor does not jump while typing.

## Why bother?

- **Testable**: `PersonViewModel` can be unit-tested without a single widget, e.g.
  `vm.setEmail("nope"); QVERIFY(!vm.isValid());`.
- **Replaceable View**: the ViewModel exposes plain properties, so the same design drives a QML View.
  Example [02](../02-qml-mvvm-dialog) does exactly that.
- **Clear ownership of state**: the dialog never edits the model directly, so Cancel is free.

## Files

```
src/
  Person.h               Model
  PersonViewModel.h/.cpp ViewModel
  PersonDialog.h/.cpp    View (QDialog)
  MainWindow.h/.cpp      shows the person and opens the dialog
  main.cpp
```
