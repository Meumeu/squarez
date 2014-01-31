import QtQuick 2.0
import Sailfish.Silica 1.0

ProgressBar
{
    property var rules
    property bool round: false
    minimumValue: 0
    maximumValue: 1
    value: 1
    Timer
    {
        interval: 100; repeat: true
        running: !rules.pause
        onTriggered: {
            parent.value = round ? rules.roundPercentageLeft : rules.percentageLeft
        }
    }
}

