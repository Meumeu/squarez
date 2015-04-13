import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0

Dialog
{
	property string coverText: ""

	onOpened: {
		playOnline.checked = Settings.value("online", true);
		url.text = Settings.value("url", "http://squarez-beta.meumeu.org/");
		playerName.text = Settings.value("playerName", "");
	}

	onAccepted: {
		Settings.setValue("online", playOnline.checked);
		Settings.setValue("url", url.text);
		Settings.setValue("playerName", playerName.text);
	}

	SilicaFlickable {
		anchors.fill: parent
		VerticalScrollDecorator {}

		contentHeight: column.height

		Column {
			id: column
			width: parent.width

			PageHeader {
				title: qsTr("Settings")
			}

			TextSwitch {
				id: playOnline
				width: parent.width
				text: qsTr("Play online")
				//description: ""
			}

			TextField {
				id: url
				width: parent.width
				label: qsTr("Server URL")
				placeholderText: label
				inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
				readOnly: !playOnline.checked
			}

			TextField {
				id: playerName
				width: parent.width
				label: qsTr("Player name")
				placeholderText: label
				inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
			}
		}
	}
}
