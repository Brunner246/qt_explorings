pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: control

    required property string title
    required property ItemTableModel tableModel
    required property DragGhost dragGhost

    readonly property bool accepting: dropArea.containsDrag && !tableModel.isFull
    readonly property bool rejecting: dropArea.containsDrag && tableModel.isFull

    color: palette.base
    radius: 4
    border.width: dropArea.containsDrag ? 2 : 1
    border.color: rejecting ? "#c0392b" : accepting ? palette.highlight : palette.mid

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 6
        spacing: 4

        Label {
            text: control.title
            font.bold: true
        }

        HorizontalHeaderView {
            syncView: table
            clip: true
            Layout.fillWidth: true
        }

        TableView {
            id: table

            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            model: control.tableModel
            boundsBehavior: Flickable.StopAtBounds
            interactive: false
            columnWidthProvider: column => column === 0 ? width * 0.6 : width * 0.4
            onWidthChanged: forceLayout()

            ScrollBar.vertical: ScrollBar {}

            delegate: Rectangle {
                id: cell

                required property int row
                required property string display
                required property string name

                implicitHeight: 30
                color: cell.row % 2 ? control.palette.alternateBase : control.palette.base

                Label {
                    anchors.fill: parent
                    leftPadding: 8
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                    text: cell.display
                }

                DragHandler {
                    id: dragHandler
                    target: null
                    onActiveChanged: {
                        if (active)
                            control.dragGhost.start(control.tableModel, cell.row, cell.name, centroid.scenePosition)
                        else
                            control.dragGhost.finish()
                    }
                    onCentroidChanged: {
                        if (active)
                            control.dragGhost.moveTo(centroid.scenePosition)
                    }
                }
            }
        }
    }

    DropArea {
        id: dropArea

        anchors.fill: parent
        keys: ["application/x-qtexamples-item"]
        onEntered: drag => drag.accepted = (drag.source as DragGhost)?.sourceModel !== control.tableModel
        onDropped: drop => {
            const ghost = drop.source as DragGhost
            if (ghost && ghost.sourceModel.moveRowTo(ghost.sourceRow, control.tableModel))
                drop.accept()
        }
    }
}
