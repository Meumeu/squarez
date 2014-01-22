import QtQuick 2.0
import Sailfish.Silica 1.0

ProgressBar
{
    property var rules
    minimumValue: 0
    maximumValue: 1
    value: 1
    Timer
    {
        interval: 30; repeat: true
        running: !rules.pause
        onTriggered: parent.value = rules.percentageLeft
    }
}

