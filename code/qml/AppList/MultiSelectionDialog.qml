import QtQuick 1.1
import com.nokia.symbian 1.1

CommonDialog {
    id: updateDialog
    height: content.height

    buttonTexts: ["Ok"]

    content:
    ListView {
        id: listView
        width: parent.width - 20
        anchors.left: parent.left
        anchors.leftMargin: 10
        contentWidth: parent.width - 20
        //contentHeight: updateDialogText.height + 20 + (checkBoxItem.visible ? checkBoxItem.height+10 : 0)
        height: updateDialog.platformContentMaximumHeight //contentHeight > updateDialog.platformContentMaximumHeight ? updateDialog.platformContentMaximumHeight : contentHeight
        flickableDirection: Flickable.VerticalFlick
        clip: true
        interactive: true //contentHeight > updateDialog.platformContentMaximumHeight ? true : false

        model: l.languageCodes

        delegate: Item {
            width: parent.width
            height: checkBox.height
            Text {
                text: l.getLanguageName(index)
                color: platformStyle.colorNormalLight
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }

            CheckBox {
                id: checkBox
                text: ""
                anchors.right: parent.right
                anchors.rightMargin: 10
                platformInverted: true
                checked: l.getSelected(modelData)
                onClicked: {
                    if (checked) l.addLanguage(modelData)
                    else l.removeLanguage(modelData)
                }
            }
        }
    }
}
