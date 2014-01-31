import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog
{
    property alias name: nameField.text
    property alias title: header.title
    property string coverText: ""
    id: dialog

    Column
    {
        anchors.fill: parent
        DialogHeader
        {
            id: header
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
            EnterKey.enabled: true
            EnterKey.onClicked: dialog.accept()
        }
    }
}
