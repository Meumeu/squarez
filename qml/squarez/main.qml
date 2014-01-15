import QtQuick 2.0
import QtQuick.Layouts 1.0

import Squarez 1.0

Rectangle
{
    SinglePlayer
    {
        id:singlePlayerRules
    }

    ColumnLayout
    {
        anchors.fill: parent
        GameArea
        {
            Layout.fillHeight: true
            Layout.fillWidth: true
            rules: singlePlayerRules
        }
        TimerArea
        {
            rules: singlePlayerRules
            Layout.fillWidth: true
            height: 20
        }
        Text
        {
            text:"Score: "+singlePlayerRules.score
        }
        HighScoresArea {}
    }
}
