/*
    PlayersBar.qml
    Displays info about players and visual clue about current player.
*/

import QtQuick 2.5
import QtQuick.Layouts 1.2

import Game 1.0

Item {
    id: playerBarRoot

    property real size: 25
    property var game: null

    RowLayout {
        anchors.fill: parent

        PlayerInfo {
            Layout.minimumWidth: size
            Layout.preferredWidth: size
            Layout.fillHeight: true

            player: game.player1
            currentPlayer: game.currentPlayer
        }

        CurrentPlayerIndicator {
            Layout.fillWidth: true
            Layout.fillHeight: true

            currentPlayer: game.currentPlayer.mark
        }

        PlayerInfo {
            Layout.minimumWidth: size
            Layout.preferredWidth: size
            Layout.fillHeight: true

            layoutDirection: Qt.RightToLeft

            player: game.player2
            currentPlayer: game.currentPlayer
        }
    }
}
