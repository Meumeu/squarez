import QtQuick 2.0

import Squarez 1.0

ListView
{
    model: HighScores {}
    delegate: Row {
        Text {text: name}
        Text {text: score}
        Text {text: date}
    }
}
