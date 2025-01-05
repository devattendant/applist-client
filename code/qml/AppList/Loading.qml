import QtQuick 1.1
import com.nokia.symbian 1.1

Item {
    id: loadingItem

    property alias text: textLoading.text

    Rectangle {
        id: darkOverlay
        color: window.black
        anchors.fill: parent
        opacity: 0.8
        visible: false

        MouseArea {
            id: overlayArea
            anchors.fill: parent
            enabled: darkOverlay.visible ? true : false
        }
    }

    BusyIndicator {
        id: indicatorLoading
        width: (parent.width < parent.height) ? parent.width/3 : parent.height/3
        height: (parent.width < parent.height) ? parent.width/3 : parent.height/3
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: false
        running: false

        Text {
            id: textLoading
            font.pixelSize: 23
            color: window.lightgrey
            z: 3
            anchors.top: indicatorLoading.bottom
            anchors.topMargin: 40
            anchors.horizontalCenter: parent.horizontalCenter
            visible: false
            text: qsTr("Loading") + "..."
        }
    }

    function start() {
        indicatorLoading.running = true
        indicatorLoading.visible = true
        textLoading.visible = true
        darkOverlay.visible = true
        overlayArea.enabled = true
    }

    function stop() {
        indicatorLoading.running = false
        indicatorLoading.visible = false
        textLoading.visible = false
        darkOverlay.visible = false
        overlayArea.enabled = false
    }
}
