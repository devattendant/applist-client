import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

Rectangle {
    id: appLinkItem

    signal clicked()
    property alias text: label.text
    property alias description: descriptionText.text
    property alias show: appLinkItem.visible
    property alias exclusiveGroup: radioButton.platformExclusiveGroup
    property alias checked: radioButton.checked

    height: show ? label.height + descriptionText.height + 20 : 0
    color: (mouseArea.pressed) ? "#4791D9" : window.white

    Text {
        id: label
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 10
        width: parent.width
        wrapMode: Text.WordWrap

        text: ""
        font.pixelSize: 23

        color: (mouseArea.pressed) ? window.white : window.black
    }

    Text {
        id: descriptionText
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: radioButton.left
        anchors.rightMargin: 10
        anchors.top: label.bottom
        anchors.topMargin: 5
        width: parent.width
        wrapMode: Text.WordWrap

        text: ""
        font.pixelSize: 18

        color: (mouseArea.pressed) ? window.white : window.black
    }

    RadioButton {
        id: radioButton
        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 5
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: if (appLinkItem.visible) tf.play()
        onReleased: if (appLinkItem.visible) tf.play()
        onClicked: if (appLinkItem.visible) radioButton.checked = true
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton
    }
}
