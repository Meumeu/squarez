import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0

Dialog
{
	property string coverText: ""

	onOpened: {
	}

	onAccepted: {
	}

	SilicaFlickable {
		anchors.fill: parent
		VerticalScrollDecorator {}

		contentHeight: column.height

		Column {
			id: column
			width: parent.width

			DialogHeader {
				title: qsTr("Settings")
			}

		}
	}
}
