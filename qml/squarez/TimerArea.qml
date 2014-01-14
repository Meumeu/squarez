import QtQuick 2.0

Rectangle
{
    property var rules
    width: 0
    height: 0
    color: "black"
    Rectangle
    {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 2
        height: parent.height - 2
        color: "white"

        Rectangle
        {
            height: parent.height
            width: parent.width
            color: "green"
            Timer
            {
                interval: 30; running: true; repeat: true
                onTriggered: parent.width = rules.percentageLeft * parent.parent.width
            }
        }
    }
}
