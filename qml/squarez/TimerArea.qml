import QtQuick 2.0

Rectangle
{
    property var rules
    property alias foreground: foregroundRectangle.color
    property alias background: backgroundRectangle.color
    width: 0
    height: 0
    color: "black"
    Rectangle
    {
        id: backgroundRectangle
        color: "#FF000000"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 2
        height: parent.height - 2

        Rectangle
        {
            id: foregroundRectangle
            color: "green"
            height: parent.height
            width: parent.width
            Timer
            {
                interval: 30; running: true; repeat: true
                onTriggered: parent.width = rules.percentageLeft * parent.parent.width
            }
        }
    }
}
