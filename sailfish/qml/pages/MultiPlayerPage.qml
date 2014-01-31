import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
    property string coverText: header.title
    property alias playerName: multiPlayerRules.playerName
    MultiPlayer
    {
        id: multiPlayerRules
        url: "http://10.0.2.2/"
    }

    Column
    {
        width: parent.width
        PageHeader
        {
            id: header
            title: "Score: " + multiPlayerRules.score
        }

        GameArea
        {
            rules: multiPlayerRules
            width: parent.width
            height: width
        }

        TimerArea
        {
            id: timer
            rules: multiPlayerRules
            round: true
            width: parent.width
        }
    }
}
