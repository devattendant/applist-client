import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1
import QtMobility.feedback 1.1

Page {
    id: settingsPage
    tools: toolBarLayout

    Rectangle {
        id: background
        color: window.white
        anchors.fill: parent
    }

    Rectangle {
        id: settingsHeader
        width: parent.width
        height: settingsHeaderText.height + 11
        color: window.white

        Text {
            id: settingsHeaderText
            text: qsTr("Settings") + l.emptyString
            width: parent.width - 20
            wrapMode: TextEdit.Wrap
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
        id: flickArea
        height: parent.height// - aboutHeader.height - basicInfo.height
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: settingsHeader.bottom
        contentWidth: parent.width
        contentHeight: settingsItem.height
        flickableDirection: Flickable.VerticalFlick
        clip: true

        Rectangle {
            id: settingsItem
            width: parent.width
            height: languageSelectionItem.height + openInBrowserItem.height + darkThemeItem.height + aasFeedItem.height + useUnsignedItem.height + aboutItem.height + 11 + 6
            color: window.white

            SelectionListItem {
                id: languageSelectionItem
                anchors.top: parent.top

                platformInverted: !window.invert

                title: (qsTr("Languages I understand") + l.emptyString)
                subTitle: l.subtitleString

                onClicked: selectionDialog.open()

                MultiSelectionDialog {
                    id: selectionDialog
                    titleText: qsTr("Languages I understand:") + l.emptyString
                }
            }

            Item {
                id: openInBrowserItem
                anchors.top: languageSelectionItem.bottom
                anchors.topMargin: 5
                width: parent.width
                height: 50

                Text {
                    text: qsTr("Open links in phone browser") + ":" + l.emptyString
                    color: window.black
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: openInBrowserSwitch.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: Text.WordWrap
                }

                Switch {
                    id: openInBrowserSwitch
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    checked: news.getOpenInBrowser()

                    onClicked: {
                        news.setOpenInBrowser(checked)
                    }
                }
            }

            Rectangle {
                id: border1
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: openInBrowserItem.bottom
                anchors.topMargin: 5
            }

            Item {
                id: darkThemeItem
                anchors.top: openInBrowserItem.bottom
                anchors.topMargin: 10
                width: parent.width
                height: 50

                Text {
                    text: qsTr("Dark theme") + ":" + l.emptyString
                    color: window.black
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: darkThemeSwitch.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: Text.WordWrap
                }

                Switch {
                    id: darkThemeSwitch
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    checked: settings.darkTheme()

                    onClicked: {
                        settings.setDarkTheme(checked)
                        infoBanner.open()
                    }
                }
            }

            Rectangle {
                id: border2
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: darkThemeItem.bottom
                anchors.topMargin: 5
            }

            Item {
                id: aasFeedItem
                anchors.top: darkThemeItem.bottom
                anchors.topMargin: 10
                width: parent.width
                height: 50

                Text {
                    text: qsTr("Show latest news from All About Symbian") + ":" + l.emptyString
                    color: window.black
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: aasFeedSwitch.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    wrapMode: Text.WordWrap
                }

                Switch {
                    id: aasFeedSwitch
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    checked: settings.aasFeed()

                    onClicked: {
                        settings.setAasFeed(checked)
                        infoBanner.open()
                    }
                }
            }

            Rectangle {
                id: border6
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: aasFeedItem.bottom
                anchors.topMargin: 5
            }

            Item {
                id: useUnsignedItem
                anchors.top: aasFeedItem.bottom
                anchors.topMargin: 5
                width: parent.width
                height: useUnsignedItemText.height + 10

                Text {
                    id: useUnsignedItemText
                    text: qsTr("Display unsigned versions (requires hacked phone or CFW)") + ":" + l.emptyString
                    color: window.black
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: useUnsignedSwitch.left
                    anchors.rightMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    wrapMode: Text.WordWrap
                }

                Switch {
                    id: useUnsignedSwitch
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter

                    checked: getList.getUseUnsigned()

                    onClicked: {
                        if (checked) {
                            unsignedDialog.open()
                        } else {
                            getList.setUseUnsigned(checked)
                            infoBanner.open()
                        }
                    }
                }
            }

            Rectangle {
                id: border7
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: useUnsignedItem.bottom
            }
        }
    }

    InfoBanner {
        id: infoBanner
        text: qsTr("This setting change will take effect after restarting AppList.")
    }


    DialogComp {
        id: unsignedDialog
        text: qsTr("Do you really want to activate displaying unsigned apps? Note that you need a hacked phone or a CFW with enabled InstallServer patch to install unsigned apps, otherwise you will get a certificate error.<br><br>Here is one of various tutorials on how to hack your Symbian phone that you can find on the internet: %1<br><br>Note that you are hacking your phone on your own risk and you may lose your (theoretical) waranty.").arg("http://bit.ly/bellehack") + l.emptyString
        buttonTexts: [qsTr("Yes"), qsTr("No")]
        titleText: "Note"
        displayCheckBox: false

        onButtonClicked: {
            if (index === 0) {
                getList.setUseUnsigned(useUnsignedSwitch.checked)
                infoBanner.open()
            } else {
                useUnsignedSwitch.checked = !useUnsignedSwitch.checked
            }
        }

        onCheckBoxCheckedChanged: {
            settings.setUnsignedDialog(!checkBoxChecked)
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }
}
