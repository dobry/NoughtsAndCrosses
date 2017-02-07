/*
  NextRoundBox.qml
  Informs about the winner and allows to restart game for next round.
*/

import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import Game 1.0

Rectangle {
    id: nextRoundBoxRoot

    border.color: "black"
    border.width: 1
    radius: 2

    opacity: 0

    visible: game.state === NoughtsAndCrosses.End || game.state === NoughtsAndCrosses.Tie

    property var game: null
    property real cellSize: 30
    property real innerRatio: 0.8

    onVisibleChanged: {
        // start opacity animation when visible or reset opacity to 0 for the next animation
        if (visible) {
            animator.running = true;
        }
        else {
            opacity = 0;
        }
    }

    OpacityAnimator {
        id: animator

        target: nextRoundBoxRoot
        from: 0
        to: 0.95
        duration: 500
    }

    Column {
        anchors.fill: parent
        anchors.margins: cellSize / 8
        spacing: cellSize / 8

        Text {
            anchors.horizontalCenter: parent.horizontalCenter

            text: game.state === NoughtsAndCrosses.Tie
                  ? "Tie!"
                  : game.state === NoughtsAndCrosses.End && game.winner
                    ? "Player #" + game.winner.index + " won!"
                    : ""

            height: cellSize / 4

            font.pixelSize: height * innerRatio
            horizontalAlignment: Text.AlignHCenter
            renderType: Text.NativeRendering
        }

        Button {
            id: nextRoundButton
            text: "One more game"

            height: cellSize / 4
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter

            // custom label style to make the font bigger
            style: ButtonStyle {
                label: Item {
                    implicitWidth: text.implicitWidth
                    implicitHeight: text.implicitHeight
                    baselineOffset: text.y + text.baselineOffset

                    Text {
                        id: text

                        anchors.centerIn: parent

                        text: control.text

                        font.pixelSize: control.height / 2
                        horizontalAlignment: Text.AlignHCenter
                        renderType: Text.NativeRendering
                    }
                }
            }

            onClicked: {
                game.nextRound();
            }
        }
    }
}
