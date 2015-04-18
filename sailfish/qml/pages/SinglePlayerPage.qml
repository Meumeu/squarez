import QtQuick 2.2
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
	property string coverText: header.title
	property alias playerName: rules.playerName
	property alias rulesType: rules.type
	id: page

	function networkError()
	{
		message.text = qsTr("Network error: score will not be saved")
	}

	Component.onCompleted: {
		rules.onNetworkError.connect(networkError);
	}

	Rules {
		id: rules
		url: "http://squarez-beta.meumeu.org/"
		paused: !applicationActive || status !== PageStatus.Active

	}

	Column {
		id: column
		width: parent.width
		PageHeader
		{
			id: header
			title: qsTr("Score: %1").arg(rules.score)
		}

		GameArea
		{
			rules: rules
			width: parent.width
			height: width

			BusyIndicator {
				anchors.centerIn: parent
				running: !rules.ready
				size: BusyIndicatorSize.Large
			}
		}

		TimerArea
		{
			id: timer
			rules: rules
			width: parent.width
		}

		Text {
			id: message
			anchors.left: parent.left
			anchors.right: parent.right
			color: Theme.primaryColor
			font.family: Theme.fontFamily
			font.pixelSize: Theme.fontSizeMedium
			wrapMode: Text.Wrap
			text: "test"
			horizontalAlignment: Text.AlignHCenter

			SequentialAnimation {
				id: messageFadeOut
				PauseAnimation {
					duration: 2000
				}
				NumberAnimation {
					target: message
					property: "opacity"
					from: 1
					to: 0
					duration: 1000
				}
			}

			onTextChanged: messageFadeOut.restart()
		}
	}

}
