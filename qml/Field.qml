/*
    Field.qml
    Displays state and allows interaction with one field on the game map.
*/

import QtQuick 2.5

import Game 1.0

Item {
    id: fieldRoot

    enabled: model.mark === Player.None

    property var model: null
    property int idx: -1

    property real marginValue: 0

    // Function used to mark the field and advance game logic
    property var markField: null

    Rectangle {
        id: fieldFace
        width: fieldRoot.width - marginValue
        height: fieldRoot.height - marginValue
        anchors.centerIn: parent

        radius: 2
        color: model.mark !== Player.None
                ? "darkgreen"
                : fieldMA.pressed
                    ? "	lightslategray"
                    : fieldMA.containsMouse
                        ? "steelblue"
                        : "green"

        MouseArea {
            id: fieldMA
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                fieldRoot.markField(fieldRoot.idx);
            }
        }

        Image {
            id: markImage
            anchors.fill: parent
            source: model.mark === Player.Nought
                    ? "qrc:images/nought.png"
                    : model.mark === Player.Cross ? "qrc:images/cross.png" : ""
            sourceSize.width: parent.width
            sourceSize.height: parent.height

            OpacityAnimator {
                target: markImage
                from: 0
                to: 1
                duration: 1000
                running: model.mark !== Player.None
            }
        }
    }
}
