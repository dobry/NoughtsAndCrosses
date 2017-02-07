/*
  GameMap.qml
  Component contains whole grid of game fields. It also draws lines indicating winning sequences.
  */

import QtQuick 2.5

import Game 1.0

Item {
    id: gameMapRoot

    property var game: null

    property real cellSize: 30
    property real marginRatio: 0

    GridView {
        id: mapGrid
        anchors.fill: parent

        enabled: game.state !== NoughtsAndCrosses.End

        cellWidth: cellSize
        cellHeight: cellSize

        // Disable flicking behavior, we only want the layout of GridView
        interactive: false

        model: game.map

        delegate: Field {
            width: cellSize
            height: cellSize

            model: modelData
            idx: index

            marginValue: cellSize * marginRatio

            markField: game.markField
        }
    }
}
