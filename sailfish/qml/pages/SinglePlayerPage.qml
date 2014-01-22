import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
    property string coverText: header.title
    SinglePlayer
    {
        id: singlePlayerRules
        pause: !applicationActive || status !== PageStatus.Active
    }

    Column
    {
        width: parent.width
        PageHeader
        {
            id: header
            title: "Score: " + singlePlayerRules.score
        }

        GameArea
        {
            rules: singlePlayerRules
            width: parent.width
            height: width
            color: "#00000000"
        }

        TimerArea
        {
            id: timer
            rules: singlePlayerRules
            width: parent.width
        }
    }
}
