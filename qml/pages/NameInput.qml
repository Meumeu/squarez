import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog
{
    property alias name: nameField.text

    Column
    {
        anchors.fill: parent
        DialogHeader
        {
            title: "Save high score"
        }
        Label
        {
            text: "Enter your name"
        }

        TextField
        {
            focus: true
            id: nameField
            placeholderText: "Your name"
        }
    }
}
