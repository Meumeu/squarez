import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
	 ListModel {
		  ListElement {
				text: QT_TR_NOOP("Squarez rules\n(click to continue)")
		  }
		  ListElement {
				text: QT_TR_NOOP("Spot similar elements at the edges of a square shape")
		  }
		  ListElement {
				text: QT_TR_NOOP("Bigger squares give more points")
		  }
		  ListElement {
				text: QT_TR_NOOP("Squares not aligned with the grid give double score")
		  }
		  ListElement {
				text: QT_TR_NOOP("Time is limited\nEach square you select refills time based on its score")
		  }
		  ListElement {
				text: QT_TR_NOOP("Total amount of available time gets shorter as you progress")
		  }
	 }

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
				title: qsTr("Score: %1").arg(tutorialRules.score)
		}

		GameArea
		{
			rules: tutorialRules
			width: parent.width
			height: width

				pixelSize: Theme.fontSizeExtraLarge
				textColor: Theme.primaryColor
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
			text: qsTr(tutorialRules.message)
			horizontalAlignment: Text.AlignHCenter
		}
	}

	MouseArea
	{
		anchors.fill: parent
		  onClicked: if (tutorialRules.message !== "") tutorialRules.next()
	}

	 Timer
	 {
		  interval: 500;
		  repeat: true
		  running: tutorialRules.message === "" && applicationActive && status === PageStatus.Active
		  onTriggered: tutorialRules.next()
	 }
}
