import QtQuick
import QtQuick.Controls

// A single floating item that follows the pointer while a row is dragged.
// It carries the drag payload (source model + row) to the DropArea.
Rectangle {
    id: ghost

    property ItemTableModel sourceModel
    property int sourceRow: -1

    function start(model: ItemTableModel, row: int, text: string, scenePosition: point) {
        sourceModel = model
        sourceRow = row
        label.text = text
        moveTo(scenePosition)
        Drag.active = true
    }

    function moveTo(scenePosition: point) {
        const local = parent.mapFromItem(null, scenePosition)
        x = local.x - width / 2
        y = local.y - height / 2
    }

    function finish() {
        Drag.drop()
        sourceModel = null
        sourceRow = -1
    }

    visible: Drag.active
    z: 100
    width: 180
    height: 32
    radius: 4
    color: palette.highlight
    opacity: 0.9

    Drag.keys: ["application/x-qtexamples-item"]
    Drag.hotSpot.x: width / 2
    Drag.hotSpot.y: height / 2

    Label {
        id: label
        anchors.fill: parent
        leftPadding: 8
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        color: ghost.palette.highlightedText
    }
}
