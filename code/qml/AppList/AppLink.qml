import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

Rectangle {
    id: appLinkItem

    signal clicked()
    property alias text: label.text
    property alias show: appLinkItem.visible
    property alias showArrows: arrows.visible

    height: show ? label.height + 34 : 0
    color: (mouseArea.pressed) ? "#4791D9" : window.white

    Text {
        id: label
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width
        wrapMode: Text.WordWrap

        text: ""
        font.pixelSize: 25

        color: (mouseArea.pressed) ? window.white : window.black
    }

    Text {
        id: arrows
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        visible: true

        text: ">>"
        font.pixelSize: 25

        color: (mouseArea.pressed) ? window.white : window.black
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: if (appLinkItem.visible) tf.play()
        onReleased: if (appLinkItem.visible) tf.play()
        onClicked: if (appLinkItem.visible) appLinkItem.clicked()
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton
    }
}
