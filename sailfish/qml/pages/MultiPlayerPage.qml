import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
    id: page
    property string coverText: header.title
    property alias playerName: multiPlayerRules.playerName
    Settings {id: settings}
    MultiPlayer
    {
        id: multiPlayerRules
        url: settings.url
    }

    Column
    {
        id: content
        width: parent.width
        PageHeader
        {
            id: header
            title: "Score: " + multiPlayerRules.score
        }

        GameArea
        {
            rules: multiPlayerRules
            width: parent.width
            height: width
        }

        TimerArea
        {
            id: timer
            rules: multiPlayerRules
            round: true
            width: parent.width
        }
    }
    SilicaListView {
        model: multiPlayerRules.highScores;
        anchors.top: content.bottom
        anchors.bottom: page.bottom
        anchors.left: page.left
        anchors.right: page.right
        delegate: BackgroundItem {
            id: delegate
            highlighted: name === multiPlayerRules.playerName && score == multiPlayerRules.score
            height: Theme.fontSizeSmall + Theme.paddingSmall

            Label {
                x: Theme.paddingLarge
                font.pixelSize: Theme.fontSizeSmall
                text: 1+index + ": " + name
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            Label
            {
                width: parent.width - Theme.paddingLarge
                font.pixelSize: Theme.fontSizeSmall
                text: score
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
            }
        }

        VerticalScrollDecorator {}
    }
}
