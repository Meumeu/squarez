import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0

Dialog
{
	property string coverText: ""
	id: dialog
	acceptDestination: Qt.resolvedUrl("SinglePlayerPage.qml")
	acceptDestinationAction: PageStackAction.Replace

	onAccepted: {
		dialog.acceptDestinationInstance.playerName = playerName.text;
		if (playOnline.checked)
			dialog.acceptDestinationInstance.rulesType = "onlineSinglePlayer";
		else
			dialog.acceptDestinationInstance.rulesType = "singlePlayer";

		Settings.setValue("online", playOnline.checked);
		Settings.setValue("playerName", playerName.text);
	}

	onOpened: {
		playOnline.checked = Settings.value("online", true);
		playerName.text = Settings.value("playerName", "");
	}

	canAccept: playerName.text !== ""

	Column
	{
		anchors.fill: parent

		DialogHeader {
			acceptText: qsTr("Start")
		}

		TextSwitch {
			id: playOnline
			width: parent.width
			text: qsTr("Play online")
		}

		TextField
		{
			id: playerName
			width: parent.width
			focus: true
			placeholderText: qsTr("Enter your name")
			EnterKey.enabled: true
			EnterKey.onClicked: dialog.accept()
		}
	}
}
