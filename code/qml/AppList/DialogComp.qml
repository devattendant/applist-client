import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    id: updateDialog
    height: content.height
    property alias text: updateDialogText.text
    property alias displayCheckBox: checkBoxItem.visible
    property alias checkBoxText: checkBoxText.text
    property alias checkBoxChecked: checkBox.checked

    content:
        Flickable {
        id: flickableUpdateDialog
        width: parent.width - 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        contentWidth: parent.width - 20
        contentHeight: updateDialogText.height + 20 + (checkBoxItem.visible ? checkBoxItem.height+10 : 0)
        height: contentHeight > updateDialog.platformContentMaximumHeight ? updateDialog.platformContentMaximumHeight : contentHeight
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: contentHeight > updateDialog.platformContentMaximumHeight ? true : false

        Text {
            id: updateDialogText

            color: "#FCFCFC"
            wrapMode: Text.WordWrap
            width: parent.width
            anchors.top: parent.top
            anchors.topMargin: 10

            text: ""
        }

        Item {
            id: checkBoxItem
            width: parent.width
            height: checkBox.height
            anchors.top: updateDialogText.bottom
            anchors.topMargin: 10
            visible: false
            Text {
                id: checkBoxText
                text: ""
                color: platformStyle.colorNormalLight
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            CheckBox {
                id: checkBox
                text: ""
                checked: false
                anchors.right: parent.right
            }
        }
    }
}
