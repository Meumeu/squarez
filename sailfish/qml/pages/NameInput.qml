import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog
{
	property alias name: nameField.text
	property string coverText: ""
	id: dialog
	acceptDestination: Qt.resolvedUrl("SinglePlayerPage.qml")
	acceptDestinationAction: PageStackAction.Replace

	onAccepted: {
		dialog.acceptDestinationInstance.playerName = dialog.name;
	}

	canAccept: nameField.text !== ""

	Column
	{
		anchors.fill: parent
		DialogHeader
		{
			id: header
			title: qsTr("Enter your name")
		}

		TextField
		{
			id: nameField
			width: parent.width
			focus: true
			placeholderText: qsTr("Your name")
			EnterKey.enabled: true
			EnterKey.onClicked: dialog.accept()
		}
	}
}
