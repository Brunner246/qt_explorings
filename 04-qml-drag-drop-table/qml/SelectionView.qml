import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Pane {
    id: root

    required property SelectionViewModel viewModel

    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 8

            ItemTable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("Available items")
                tableModel: root.viewModel.availableItems
                dragGhost: ghost
            }

            ItemTable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                title: qsTr("Selected items")
                tableModel: root.viewModel.selectedItems
                dragGhost: ghost
            }
        }

        RowLayout {
            Layout.fillWidth: true

            Label { text: qsTr("Maximum items:") }
            SpinBox {
                from: 1
                to: 50
                editable: true
                value: root.viewModel.capacity
                onValueModified: root.viewModel.capacity = value
            }
            Item { Layout.fillWidth: true }
            Label {
                text: root.viewModel.summary
                color: root.viewModel.isFull ? "#c0392b" : root.palette.windowText
                font.bold: root.viewModel.isFull
            }
        }
    }

    DragGhost {
        id: ghost
    }
}
