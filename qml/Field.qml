/*
    Field.qml
    Displays state and allows interaction with one field on the game map.
*/

import QtQuick 2.5

import Game 1.0

Item {
    id: fieldRoot

    property var model: null
    property int idx: -1

    // Function used to mark the field and advance game logic
    property var markField: function () {
        console.error("");
    }

    Rectangle {
        id: fieldFace
        width: fieldRoot.width - marginValue
        height: fieldRoot.height - marginValue
        anchors.centerIn: parent

        radius: 2
        color: fieldMA.containsMouse ? "steelblue" : "green"

        MouseArea {
            id: fieldMA
            anchors.fill: parent
            hoverEnabled: true

            onClicked: {
                fieldRoot.markField(fieldRoot.idx);
            }
            enabled: model.mark === Player.None
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