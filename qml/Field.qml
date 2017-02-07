/*
    Field.qml
    Displays state and allows interaction with one field on the game map.
*/

import QtQuick 2.5

import Game 1.0

Item {
    id: fieldRoot

    enabled: model.mark === Player.None

    property var currentPlayer: null
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
                    ? "darkgreen"
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

            anchors {
                fill: parent
                margins: marginValue
            }

            property string noughtSource: "qrc:images/nought.png"
            property string crossSource: "qrc:images/cross.png"

            source: {
                // if field is hovered show ghost (image + opacity) of currentPlayer's mark
                if (model.mark === Player.None && fieldMA.containsMouse) {
                    return currentPlayer.mark === Player.Nought ? noughtSource : crossSource;
                }
                // otherwise just show assigned mark
                else {
                    return model.mark === Player.Nought
                        ? noughtSource
                        : model.mark === Player.Cross ? crossSource : ""
                }
            }
            opacity: 0.1

            sourceSize.width: parent.width - marginValue * 2
            sourceSize.height: parent.height - marginValue * 2

            OpacityAnimator {
                target: markImage
                from: 0.1
                to: 1
                duration: 1000
                running: model.mark !== Player.None
            }

            Connections {
                target: fieldRoot.model
                // reset opacity when field is reset to Player.None
                onMarkChanged: {
                    if (fieldRoot.model.mark === Player.None) {
                        markImage.opacity = 0.1;
                    }
                }
            }
        }
    }
}
