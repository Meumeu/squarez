import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
	property string coverText: header.title
	Rules {
		id: tutorialRules
		type: "tutorial"
	}

	Column
	{
		width: parent.width
		PageHeader
		{
			id: header
			title: "Score: " + tutorialRules.score
		}

		GameArea
		{
			rules: tutorialRules
			width: parent.width
			height: width
		}

		TimerArea
		{
			id: timer
			rules: tutorialRules
			width: parent.width
		}

		Text
		{
			width: parent.width - x * 2
			x: Theme.paddingMedium * 2
			color: Theme.primaryColor
			font.family: Theme.fontFamily
			font.pixelSize: Theme.fontSizeSmall
			wrapMode: Text.Wrap
			text: tutorialRules.message
		}
	}
	MouseArea
	{
		anchors.fill: parent
		onClicked: tutorialRules.next()
	}
}
