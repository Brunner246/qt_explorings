# 02 – MVVM with the View in QML (QQuickWidget)

The same application as [01](../01-widgets-mvvm-dialog), but the form inside the dialog is written in QML.
**Model and ViewModel are unchanged.** Only the View is different, which is the main point of MVVM.

```
QDialog (PersonDialog)
├── QQuickWidget  ──▶  PersonForm.qml  ──binds to──▶  PersonViewModel (C++)
└── QDialogButtonBox (regular widgets)
```

The dialog mixes both worlds: the form is QML, while the OK/Cancel/Reset buttons are ordinary widgets connected
to the same ViewModel.

## Exposing the ViewModel to QML

`PersonViewModel` is registered at compile time:

```cpp
class PersonViewModel : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Provided by PersonDialog")
    ...
```

`qt_add_qml_module(... URI MvvmQml ...)` in `CMakeLists.txt` generates the type registration and compiles
`PersonForm.qml` into the executable. QML can then use `PersonViewModel` as a typed property, and `qmllint`
and the QML compiler know all of its properties.

## Injecting the instance

`PersonForm.qml` declares what it needs:

```qml
required property PersonViewModel viewModel
```

`PersonDialog::loadForm()` creates the component with that property already set:

```cpp
QQmlComponent component(m_quickWidget->engine());
component.loadFromModule("MvvmQml", "PersonForm");
m_form.reset(qobject_cast<QQuickItem *>(component.createWithInitialProperties({{"viewModel", ...}})));
m_form->setParentItem(m_quickWidget->quickWindow()->contentItem());
```

Why not `QQuickWidget::setSource()`?

- In Qt 6.8, `QQuickWidget` has no `setInitialProperties()`. With `setSource()`, the object would be created
  first and the ViewModel set afterwards. Every binding would then need a null check (`viewModel?.name ?? ""`).
- Context properties (`rootContext()->setContextProperty(...)`) are discouraged. They are untyped, invisible
  to tooling and slow to look up.

With `createWithInitialProperties()`, the `required` property is set before any binding is evaluated, so the QML
stays clean. The root item uses `anchors.fill: parent` to follow the size of the widget.

## Two-way binding in QML

```qml
TextField {
    text: root.viewModel.name               // ViewModel → View
    onTextEdited: root.viewModel.name = text // View → ViewModel
}
```

`onTextEdited`/`onValueModified`/`onActivated` only fire on user interaction, so changes coming from the
ViewModel (for example **Reset**) do not loop back.

## Files

```
src/
  Person.h, PersonViewModel.h/.cpp   identical to 01, plus QML_ELEMENT
  PersonDialog.h/.cpp                QDialog hosting the QQuickWidget
  MainWindow.h/.cpp, main.cpp
qml/
  PersonForm.qml                     the View
```
