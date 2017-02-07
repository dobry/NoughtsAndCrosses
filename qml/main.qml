import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.1

import Game 1.0

Window {
    id: windowRoot
    visible: true

    width: 600
    height: 800

    property var game: NoughtsAndCrosses { }

    property real cellSize: windowRoot.width / 3
    property real playerBarHeight: cellSize / 3
    property real innerRatio: 0.8
    property real marginRatio: (1 - innerRatio) / 2
    property real marginValue: cellSize * marginRatio

    ColumnLayout {
        anchors {
            fill: parent
            topMargin: marginValue
        }

        PlayersBar {
            anchors {
                left: parent.left
                right: parent.right
                leftMargin: marginValue
                rightMargin: marginValue
            }
            Layout.preferredHeight: playerBarHeight
            Layout.minimumHeight: playerBarHeight

            size: cellSize - 2 * marginValue
            game: windowRoot.game
        }

        GameMap {
            anchors {
                left: parent.left
                right: parent.right
            }
            Layout.fillHeight: true

            game: windowRoot.game

            cellSize: windowRoot.cellSize
            marginRatio: windowRoot.marginRatio
        }
    }

    NextRoundBox {
        width: parent.width * innerRatio
        height: cellSize
        anchors.centerIn: parent

        game: windowRoot.game

        cellSize: windowRoot.cellSize
        innerRatio: windowRoot.innerRatio
    }
}
