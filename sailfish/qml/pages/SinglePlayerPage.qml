import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
	property string coverText: header.title
	property alias playerName: singlePlayerRules.playerName

	Rules {
		id: singlePlayerRules
		type: "onlineSinglePlayer"
// 		type: "singlePlayer"
		url: "http://heracles/"
		paused: !applicationActive || status !== PageStatus.Active
	}

	Column
	{
		width: parent.width
		PageHeader
		{
			id: header
			title: qsTr("Score: %1").arg(singlePlayerRules.score)
		}

		GameArea
		{
			rules: singlePlayerRules
			width: parent.width
			height: width

			pixelSize: Theme.fontSizeExtraLarge
				textColor: Theme.primaryColor

			BusyIndicator {
				anchors.centerIn: parent
				running: !singlePlayerRules.ready
				size: BusyIndicatorSize.Large
			}
		}

		TimerArea
		{
			id: timer
			rules: singlePlayerRules
			width: parent.width
		}
	}
}
