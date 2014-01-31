import QtQuick 2.0
import Sailfish.Silica 1.0

import harbour.squarez 1.0
import "../squarez"

Page
{
    id: page
    property string coverText: header.title
    property alias playerName: multiPlayerRules.playerName
    MultiPlayer
    {
        id: multiPlayerRules
        url: "http://10.0.2.2/"
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
        anchors.top: content.anchors.bottom
        anchors.bottom: page.anchors.bottom
        anchors.left: page.anchors.left
        anchors.right: page.anchors.right
        delegate: BackgroundItem {
            id: delegate

            Label {
                x: Theme.paddingLarge
                text: name
                anchors.verticalCenter: parent.verticalCenter
                color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
            }
            Label
            {
                width: parent.width - Theme.paddingLarge
                text: score
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignRight
            }
        }

        VerticalScrollDecorator {}
    }
}
