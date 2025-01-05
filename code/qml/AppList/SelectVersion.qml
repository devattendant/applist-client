import QtQuick 1.1
import com.nokia.symbian 1.1
import Qt.labs.components 1.1

Page {
    id: selectVersion
    tools: toolBarLayoutSelectVersion

    property string downloadUrl: ""
    property string downloadStoreUrl: ""
    property string downloadUnsignedUrl: ""

    property string version: ""
    property string versionStore: ""
    property string versionUnsigned: ""

    property string versiondate: ""
    property string versiondateStore: ""
    property string versiondateUnsigned: ""

    property string unsignedNote: ""
    property string installed: ""
    property bool sameUid: false // don't force to uninstall if selfsigned and unsigned UID are equal
    property bool download: false // damit update = true

    Rectangle {
        id: bg
        anchors.fill: parent
        color: window.white
    }

    Rectangle {
        id: header
        width: parent.width
        height: headerText.height + 17
        color: window.white
        z: 3

        Text {
            id: headerText
            text: qsTr("Select the version you want to install. AppList will help you to upgrade between different releases if required.") + l.emptyString
            width: parent.width - 20
            wrapMode: TextEdit.WordWrap
            font.pixelSize: 18
            color: window.black
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        Rectangle {
            id: border
            height: 1
            width: parent.width
            color: window.black
            anchors.bottom: parent.bottom
        }
    }

    Flickable {
        id: flickable
        anchors.top: header.bottom
        anchors.bottom: continueButtonRect.top
        width: parent.width
        contentWidth: parent.width
        contentHeight: itemSelfSigned.height + itemStore.height + itemUnsigned.height
        flickableDirection: Flickable.VerticalFlick
        clip: true
        z: 2

        /*
          Download: Alle drei
          Update von
          -Unsigned: Direkt (nur unsigned)
          -Store: Self-Signed oder Unsigned
          -Selfsigned: Direkt (Selfsigned)
          (stimmt nicht ^^)
          */

        CheckableGroup {
            id: group
        }

        SelectVersionItem {
            id: itemSelfSigned
            width: parent.width
            text: "Self-Signed <small>("+version+", "+versiondate+")</small>"
            description: qsTr("In most cases the self-signed version should be enough for you, although it comes with limited possibilities for the developer. Have a look at the unsigned version if available to check out the differences. Ask the developer if you are unsure.")
            show: (downloadUrl !== "")
            visible: show
            anchors.top: parent.top
            exclusiveGroup: group
        }

        SelectVersionItem {
            id: itemStore
            width: parent.width
            text: "Store <small>("+versionStore+", "+versiondateStore+")</small>"
            description: qsTr("With installing the Store version you won't have any problems while installing and can be sure that the content was tested (by a more or less good QA team...). BUT note that developers can't update their apps in the Nokia Store since 1.1.2014 and you should prefer the self-signed or unsigned versions in any cases when available.")
            show: (downloadStoreUrl !== "") && download && (downloadUrl === "")
            visible: show
            anchors.top: itemSelfSigned.bottom
            exclusiveGroup: group
        }

        SelectVersionItem {
            id: itemUnsigned
            width: parent.width
            text: "Unsigned <small>("+versionUnsigned+", "+versiondateUnsigned+")</small>"
            description: qsTr("The unsigned version of an app has the most available features, but is not signed with a certificate. To be able to install an unsigned app you need to have a hacked phone or a CFW with InstallServer.exe modified.") + ((unsignedNote !== "") ? ("<br><br>"+qsTr("Notes for this unsigned version by the developer")+":<br>" + unsignedNote) : "")
            show: (downloadUnsignedUrl !== "") && getList.getUseUnsigned()
            visible: show
            anchors.top: itemStore.bottom
            exclusiveGroup: group
        }
    }

    Rectangle {
        id: continueButtonRect
        width: parent.width
        height: continueButton.height + 17
        anchors.bottom: parent.bottom
        color: window.white
        z: 3

        Rectangle {
            id: border2
            height: 1
            width: parent.width
            color: window.black
            anchors.top: parent.top
        }

        Button {
            id: continueButton
            anchors.centerIn: parent
            text: qsTr("Continue...")
            width: parent.width/2

            onClicked: {
                if (itemSelfSigned.checked) {
                    if (installed.charAt(installed.length - 1) == '1' || download) {
                        // Installed version is selfsigned
                        // Do a direct update
                        getList.buttonPressed(1)
                        detailPage.backFromSelectVersion(installed)
                        window.pageStack.pop()
                    } else {
                        updateDialog.actionAfter = 1
                        updateDialog.open()
                    }
                } else if (itemUnsigned.checked) {
                    if (installed.charAt(installed.length - 1) == '2' || download || (installed.charAt(installed.length - 1) == '1' && selectVersion.sameUid)) {
                        // Installed version is unsigned
                        // Do a direct update
                        getList.buttonPressed(2)
                        detailPage.backFromSelectVersion(installed)
                        window.pageStack.pop()
                    } else {
                        updateDialog.actionAfter = 2
                        updateDialog.open()
                    }
                } else if (itemStore.checked) {
                    if (installed.charAt(installed.length - 1) == '3' || download) {
                        // Installed version is Store
                        // Do a direct update
                        getList.buttonPressed(3)
                        detailPage.backFromSelectVersion(installed)
                        window.pageStack.pop()
                    } else {
                        updateDialog.actionAfter = 3
                        updateDialog.open()
                    }
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    //CommonDialog, informing about the requirement to uninstall Store version for any other or to enable InstallServer
    CommonDialog {
        id: updateDialog
        titleText: qsTr("Update information")
        buttonTexts: [qsTr("Rescan"), qsTr("Cancel")]
        height: content.height

        property int actionAfter: 0

        content:
            Flickable {
            id: flickableUpdateDialog
            width: parent.width - 20
            anchors.left: parent.left
            anchors.leftMargin: 10
            contentWidth: parent.width - 20
            contentHeight: updateDialogText.height + 20
            height: contentHeight > updateDialog.platformContentMaximumHeight ? updateDialog.platformContentMaximumHeight : contentHeight
            flickableDirection: Flickable.VerticalFlick
            clip: true
            interactive: contentHeight > updateDialog.platformContentMaximumHeight ? true : false

            Text {
                id: updateDialogText

                color: "#FFFFFF"
                wrapMode: Text.WordWrap
                width: parent.width
                anchors.top: parent.top
                anchors.topMargin: 10

                text: qsTr("To install the selected version you first need to uninstall the current version of this app. Find the app in the main menu, long press and select 'Uninstall' to remove it. If uninstallation was finished click 'Rescan' below to continue.")
            }
        }

        onButtonClicked: {
            if (index == 0) {
                // Rescan
                if (appManagement.checkAppInstalled(installed, true)) {
                    updateDialog.open()
                } else {
                    // App was uninstalled
                    installed = "0"
                    getList.buttonPressed(actionAfter)
                    updateDialog.close()
                    detailPage.backFromSelectVersion(installed)
                    window.pageStack.pop()
                }
            } else if (index == 1) {
                // Cancel
                updateDialog.close()
                window.pageStack.pop()
            }
        }
    }

    ToolBarLayout {
        id: toolBarLayoutSelectVersion
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: window.pageStack.pop()
        }
    }
}
