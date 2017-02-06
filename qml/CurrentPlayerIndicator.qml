/*
    CurrentPlayerIndicator.qml
    Points to the left or to the right depending on the current player's side.
*/

import QtQuick 2.5

import Game 1.0

Item {
    property int currentPlayer: Player.Nought
    Item {
        height: parent.height
        width: height
        anchors.horizontalCenter: parent.horizontalCenter
        Image {
            height: parent.height * 0.8
            anchors.verticalCenter: parent.verticalCenter

            source: "qrc:images/arrow.png"
            sourceSize.height: height

            mirror: currentPlayer === Player.Nought
            fillMode: Image.PreserveAspectFit
        }
    }
}
