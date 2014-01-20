import QtQuick 2.0
import Sailfish.Silica 1.0

import Squarez 1.0
import "../squarez"

Page
{
    SinglePlayer
    {
        id: singlePLayerRules
        pause: !applicationActive || status !== PageStatus.Active
    }

    Column
    {
        width: parent.width
        PageHeader
        {
            id: header
            title: "Score: " + singlePLayerRules.score
        }

        GameArea
        {
            rules: singlePLayerRules
            width: parent.width
            height: width
            color: "#00000000"
        }

        TimerArea
        {
            id: timer
            rules: singlePLayerRules
            width: parent.width
        }
    }
}
