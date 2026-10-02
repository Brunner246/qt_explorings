import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: root

    required property PersonViewModel viewModel

    anchors.fill: parent
    implicitWidth: 380

    GridLayout {
        anchors.fill: parent
        columns: 2
        columnSpacing: 12
        rowSpacing: 8

        Label { text: qsTr("Name:") }
        TextField {
            Layout.fillWidth: true
            text: root.viewModel.name
            onTextEdited: root.viewModel.name = text
        }

        Label { text: qsTr("Email:") }
        TextField {
            Layout.fillWidth: true
            text: root.viewModel.email
            onTextEdited: root.viewModel.email = text
        }

        Label { text: qsTr("Age:") }
        SpinBox {
            editable: true
            from: root.viewModel.minimumAge
            to: root.viewModel.maximumAge
            value: root.viewModel.age
            onValueModified: root.viewModel.age = value
        }

        Label { text: qsTr("Role:") }
        ComboBox {
            Layout.fillWidth: true
            model: root.viewModel.roleNames
            currentIndex: root.viewModel.roleIndex
            onActivated: index => root.viewModel.roleIndex = index
        }

        Label {
            Layout.columnSpan: 2
            Layout.fillWidth: true
            color: "#c0392b"
            text: root.viewModel.validationMessage
        }

        Item {
            Layout.columnSpan: 2
            Layout.fillHeight: true
        }
    }
}
