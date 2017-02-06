import QtQuick 2.5
import QtQuick.Window 2.2
import Game 1.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Window {
    id: windowRoot
    visible: true

    width: 600
    height: 800

    property var game: NoughtsAndCrosses { }

    property real cellWidth: windowRoot.width / 3
    property real cellHeight: cellWidth
    property real playerBarHeight: cellWidth / 3
    property real innerRatio: 0.8
    property real marginRatio: (1 - innerRatio) / 2
    property real marginValue: cellWidth * marginRatio

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

            size: cellWidth - 2 * marginValue
            game: windowRoot.game
        }

        GridView {
            id: mapGrid
            anchors {
                left: parent.left
                right: parent.right
            }
            Layout.fillHeight: true

            cellWidth: windowRoot.cellWidth
            cellHeight: windowRoot.cellHeight

            // Disable flicking behavior, we only want the layout of GridView
            interactive: false

            model: game.map

            delegate: Field {
                width: windowRoot.cellWidth
                height: windowRoot.cellHeight

                model: modelData
                idx: index

                markField: game.markField
            }
        }
    }
}
