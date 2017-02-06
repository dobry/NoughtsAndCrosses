/*
    PlayerInfo.qml
    Contains informations about a player in a visual form.
*/

import QtQuick 2.5
import QtQuick.Layouts 1.3

import Game 1.0

Rectangle {
    id: playerInfoRoot

    border.color: player === currentPlayer ? "orange" : "#666"
    border.width: 2

    color: "transparent"

    property var player: null
    property var currentPlayer: null
    property int layoutDirection: Qt.LeftToRight

    RowLayout {
        width: parent.width
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
        }

        layoutDirection: playerInfoRoot.layoutDirection
        spacing: 0

        Item {
            Layout.preferredWidth: parent.height
            Layout.minimumWidth: parent.height
            Layout.preferredHeight: parent.height
            Layout.minimumHeight: parent.height

            Image {
                property real size: parent.height * 0.8

                height: size
                width: size
                anchors.centerIn: parent

                source: playerInfoRoot.player.mark === Player.Nought ? "qrc:images/nought.png" : "qrc:images/cross.png"
                sourceSize.width: size
                sourceSize.height: size
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.preferredHeight: parent.height
            Layout.minimumHeight: parent.height

            Text {
                id: playerScore

                width: parent.width
                height: parent.height

                text: playerInfoRoot.player.score

                font.pixelSize: parent.height * 0.8
                horizontalAlignment: Text.AlignHCenter
                renderType: Text.NativeRendering
            }
        }
    }
}
