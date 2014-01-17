import QtQuick 2.0
import Sailfish.Silica 1.0

import Squarez 1.0
import "../squarez"

Page
{
    SinglePlayer
    {
        id: singlePLayerRules
    }

    Column
    {
        width: parent.width
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
            height: 20
            width: parent.width
            foreground: Theme.rgba(Theme.primaryColor, 0.8)
            background: "#00000000"
        }
    }
}
