import QtQuick 2.0

import Squarez 1.0

ListView
{
    model: HighScores {}
    delegate: Row {
        spacing: 5
        Text {text: name}
        Text {text: Qt.formatDate(date)}
        Text {text: score}
    }
}
