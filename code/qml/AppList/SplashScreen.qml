import QtQuick 1.1

Item {
    id: splashItem
    width: parent.width
    height: parent.height

    Rectangle {
        anchors.fill: parent
        color: "#2878C2" // darker than "#4791D9"
    }

    Behavior on opacity {
        NumberAnimation { duration: 500 }
    }

    Image {
        id: icon
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        source: "images/iconbig.png"
    }

    Text {
        id: version
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: web.top
        anchors.bottomMargin: 5
        font.pixelSize: 19
        color: "#FCFCFC"
        text: "AppList 1.0 (Build 298)"
    }

    Text {
        id: web
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        font.pixelSize: 19
        color: "#FCFCFC"
        text: "http://xxx"
    }
}
