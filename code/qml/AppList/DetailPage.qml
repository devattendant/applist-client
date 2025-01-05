import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1
import QtMobility.feedback 1.1

Page {
    id: detailPage

    tools: toolBarLayout

    property int listIndex: 0
    property variant detailItem: -1

    signal backFromSelectVersion(string installed)
    onBackFromSelectVersion: {
        // Coming back from SelectVersion
        if (installed === "0") detailItem["installed"] = "0"

        downloadProgress.visible = true
    }

    Rectangle {
        id: background
        color: window.white
        anchors.fill: parent
    }

    Flickable {
        id: flickArea
        height: parent.height// - aboutHeader.height - basicInfo.height
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top //basicInfo.bottom
        contentWidth: parent.width
        contentHeight: 74 + appName.height + appIcon.height + (downloadProgress.viaible ? downloadProgress.height : appButton.height) + appDescription.height + contactsHeader.height + appContactLinks.height + appLinks.height
        flickableDirection: Flickable.VerticalFlick
        clip: true

        function flickAreaHeight() {
            return 74 + appName.height + appIcon.height + (downloadProgress.viaible ? downloadProgress.height : appButton.height) + appDescription.height + contactsHeader.height + appContactLinks.height + appLinks.height
        }

        Label {
            id: appName
            text: detailItem["name"]
            width: parent.width - 20
            wrapMode: TextEdit.Wrap
            font.pixelSize: 30
            color: window.black
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.leftMargin: 10
        }

        Rectangle {
            id: border
            height: 1
            width: parent.width
            color: window.lightgrey
            anchors.top: appName.bottom
            anchors.topMargin: 10
        }

        Image {
            id: appIcon
            height: 80
            width: 80
            asynchronous: true
            source: detailItem["icon"]
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.top: border.bottom
            anchors.topMargin: 10
        }

        Item {
            height: appDeveloper.height + appCategory.height + appVersion.height
            anchors.left: appIcon.right
            anchors.leftMargin: 5
            anchors.verticalCenter: appIcon.verticalCenter

            Label {
                id: appDeveloper
                text: detailItem["developer"]
                wrapMode: TextEdit.Wrap
                font.pixelSize: 20
                color: window.grey
                anchors.top: parent.top
            }

            Label {
                id: appCategory
                text: l.categoryName(detailItem["category"])
                wrapMode: TextEdit.Wrap
                font.pixelSize: 20
                color: window.grey
                anchors.top: appDeveloper.bottom
                anchors.topMargin: 0
            }

            Label {
                id: appVersion
                property string sizeString: ((detailItem["versionunsigned"] !== "" && getList.getUseUnsigned()) ? (detailItem["downloadunsignedsize"] !== "0" ? ( parseInt(detailItem["downloadunsignedsize"]) > 1000000 ? (detailItem["downloadunsignedsize"]/1000000.0).toFixed(1)+" MB" : (detailItem["downloadunsignedsize"]/1000.0).toFixed()+" KB") : "") : (detailItem["version"] !== "") ? (detailItem["downloadsize"] !== "0" ? ( parseInt(detailItem["downloadsize"]) > 1000000 ? (detailItem["downloadsize"]/1000000.0).toFixed(1)+" MB" : (detailItem["downloadsize"]/1000.0).toFixed()+" KB") : "") : "")
                text: ((detailItem["versionunsigned"] !== "" && getList.getUseUnsigned()) ? detailItem["versionunsigned"] : (detailItem["version"] !== "") ? detailItem["version"] : detailItem["versionstore"]) + (sizeString != "" ? " - " + sizeString : "")
                wrapMode: TextEdit.Wrap
                font.pixelSize: 20
                color: window.grey
                anchors.top: appCategory.bottom
                anchors.topMargin: 0
            }
        }

        Label {
            id: appPrice
            text: (detailItem["price"] > 0) ? (detailItem["price"] + " €") : (qsTr("Free") + l.emptyString)
            font.pixelSize: 24
            color: window.black
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: appButton.verticalCenter
        }

        Item {
            id: downloadProgress
            anchors.top: appIcon.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            width: parent.width - 20
            visible: (appButton.buttonAction == 6) ? true : false
            height: cancelButton.height + progressBar.height + 10

            //onVisibleChanged: flickArea.contentHeight = flickArea.flickAreaHeight()

            Button {
                id: cancelButton
                text: qsTr("Cancel") + l.emptyString
                anchors.top: appIcon.bottom
                anchors.right: parent.right
                width: (detailPage.width/5) * 2
                onClicked: {
                    if (getList.cancelDownload()) {
                        downloadProgress.visible = false;
                        infoBanner.text = qsTr("Download cancelled.")
                        infoBanner.open()
                    } else {
                        infoBanner.text = qsTr("Download could not be cancelled.")
                        infoBanner.open()
                    }
                }
            }

            ProgressBar {
                id: progressBar
                width: parent.width
                anchors.top: cancelButton.bottom
                anchors.topMargin: 10
                anchors.right: parent.right
                value: getList._downloadProgress
                maximumValue: 100.0
            }
        }

        Button {
            id: appButton
            property int buttonAction: getList.buttonAction(listIndex)
            text: (buttonAction == 0 || buttonAction == 7) ? (qsTr("Download") + l.emptyString) : (buttonAction == 1 || buttonAction == 2 || buttonAction == 3 || buttonAction == 5) ? (qsTr("Update") + l.emptyString) : (qsTr("Launch") + l.emptyString)
            anchors.top: appIcon.bottom
            anchors.topMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 10
            width: (detailPage.width/5) * 2
            visible: !downloadProgress.visible
            onClicked: {
                if (buttonAction == 4) getList.buttonPressed(4)
                else if (buttonAction == 5 || buttonAction == 7) openSelectVersion()
                else {
                    // No update selection required, directly start downloading
                    // buttonPressed returns if displaying downloadProgress
                    // Shall not happen if Store version

                    // Nokia Store: open dialog if first time
                    if ((buttonAction == 3 || (buttonAction == 0 && detailItem["downloadstore"] !== "" && detailItem["download"] === "" && detailItem["downloadunsigned"] === "")) && getList.getStoreNote()) storeDialog.open()
                    // Not Nokia Store
                    else downloadProgress.visible = getList.buttonPressed(buttonAction)
                }
            }
        }

        Rectangle {
            id: border2
            height: 1
            width: parent.width
            color: window.lightgrey
            anchors.top: downloadProgress.visible ? downloadProgress.bottom : appButton.bottom
            anchors.topMargin: 10
        }

        Item {
            id: appDescription
            width: parent.width
            height: (appDescriptionLabel.paintedHeight>(detailPage.height/3)) ? (detailPage.height/3) : appDescriptionLabel.height
            anchors.top: border2.bottom
            anchors.topMargin: 10

            property bool opened: !(appDescriptionLabel.paintedHeight>(detailPage.height/3))

            Text {
                id: appDescriptionLabel
                text: ((appButton.buttonAction == 1 || appButton.buttonAction == 2 || appButton.buttonAction == 3 || appButton.buttonAction == 5) && (detailItem["changelog"] !== "")) ? detailItem["changelog"] + "\n\n" + detailItem["description"] : detailItem["description"]
                width: parent.width - 20
                wrapMode: TextEdit.Wrap
                font.pixelSize: 18
                color: window.black
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            Rectangle {
                id: appDescriptionRect
                width: parent.width
                height: parent.height/3

                anchors.bottom: parent.bottom
                anchors.bottomMargin: -10
                visible: !appDescription.opened

                gradient: Gradient {
                    GradientStop { position: 0.0; color: window.whiteTransparent }
                    GradientStop { position: 1.0; color: window.white }
                }

                MouseArea {
                    id: appDescriptionMouseArea
                    anchors.fill: parent

                    onPressed: if (!appDescription.opened || appDescriptionLabel.paintedHeight>(detailPage.height/3)) tf.play()
                    onReleased: if (!appDescription.opened || appDescriptionLabel.paintedHeight>(detailPage.height/3)) tf.play()

                    onClicked: {
                        if (!appDescription.opened) {
                            // Open label content
                            appDescription.height = appDescriptionLabel.paintedHeight
                            appDescription.opened = true
                        } else if (appDescriptionLabel.paintedHeight>(detailPage.height/3)) {
                            appDescription.height = (detailPage.height/3)
                            appDescription.opened = false
                        }
                    }
                }
            }
        }

        Rectangle {
            id: appDescriptionLayer
            visible: !appDescription.opened
            width: parent.width
            height: appDescriptionLabel.paintedHeight
            color: window.white
            anchors.top: appDescription.bottom
            anchors.topMargin: 10
        }

        Rectangle {
            id: border3
            height: 1
            width: parent.width
            color: window.lightgrey
            anchors.top: appDescription.bottom
            anchors.topMargin: 10
        }

        Rectangle {
            id: contactsHeader
            width: parent.width
            height: contactsText.height + 11
            anchors.top: border3.bottom
            color: window.white

            Text {
                id: contactsText
                text: qsTr("Contact the developer") + l.emptyString
                width: parent.width - 20
                wrapMode: TextEdit.Wrap
                font.pixelSize: 18
                color: window.black
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            Rectangle {
                id: border8
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.bottom: parent.bottom
            }
        }

        Rectangle {
            id: appContactLinks
            width: parent.width
            height: parent.width/4
            anchors.top: contactsHeader.bottom
            color: window.white

            Rectangle {
                id: appTwitter
                height: parent.width/4
                width: parent.width/4
                color: (mouseAreaTwitter.pressed && (detailItem["twitter"] !== "")) ? "#4791D9" : window.white
                anchors.left: parent.left
                anchors.top: parent.top

                Image {
                    anchors.centerIn: parent
                    asynchronous: true
                    source: "images/twitter.png"
                    opacity: (detailItem["twitter"] !== "") ? 1 : 0.3
                }

                MouseArea {
                    id: mouseAreaTwitter
                    anchors.fill: parent
                    onPressed: tf.play()
                    onReleased: tf.play()
                    onClicked: {
                        if (detailItem["twitter"] !== "") Qt.openUrlExternally("https://twitter.com/"+detailItem["twitter"])
                    }
                }
            }

            Rectangle {
                id: appFacebook
                height: parent.width/4
                width: parent.width/4
                color: (mouseAreaFacebook.pressed && (detailItem["facebook"] !== "")) ? "#4791D9" : window.white
                anchors.left: parent.left
                anchors.leftMargin: parent.width/4
                anchors.top: parent.top

                Image {
                    anchors.centerIn: parent
                    asynchronous: true
                    source: "images/facebook.png"
                    opacity: (detailItem["facebook"] !== "") ? 1 : 0.3
                }

                MouseArea {
                    id: mouseAreaFacebook
                    anchors.fill: parent
                    onPressed: tf.play()
                    onReleased: tf.play()
                    onClicked: {
                        if (detailItem["facebook"] !== "") Qt.openUrlExternally(detailItem["facebook"])
                    }
                }
            }

            Rectangle {
                id: appMail
                height: parent.width/4
                width: parent.width/4
                color: (mouseAreaMail.pressed && (detailItem["mail"] !== "")) ? "#4791D9" : window.white
                anchors.right: parent.right
                anchors.rightMargin: parent.width/4
                anchors.top: parent.top

                Image {
                    anchors.centerIn: parent
                    asynchronous: true
                    source: "images/mail.png"
                    opacity: (detailItem["mail"] !== "") ? 1 : 0.3
                }

                MouseArea {
                    id: mouseAreaMail
                    anchors.fill: parent
                    onPressed: tf.play()
                    onReleased: tf.play()
                    onClicked: {
                        if (detailItem["mail"] !== "") Qt.openUrlExternally("mailto:"+detailItem["mail"]+"?subject="+detailItem["name"]+" on AppList")
                    }
                }
            }

            Rectangle {
                id: appWebsite
                height: parent.width/4
                width: parent.width/4
                color: (mouseAreaWebsite.pressed && (detailItem["website"] !== "")) ? "#4791D9" : window.white
                anchors.right: parent.right
                anchors.top: parent.top

                Image {
                    anchors.centerIn: parent
                    asynchronous: true
                    source: "images/web.png"
                    opacity: (detailItem["website"] !== "") ? 1 : 0.3
                }

                MouseArea {
                    id: mouseAreaWebsite
                    anchors.fill: parent
                    onPressed: tf.play()
                    onReleased: tf.play()
                    onClicked: {
                        if (detailItem["website"] !== "") Qt.openUrlExternally(detailItem["website"])
                    }
                }
            }
        }

        Rectangle {
            id: border4
            height: 1
            width: parent.width
            color: window.lightgrey
            anchors.top: appContactLinks.bottom
        }

        Rectangle {
            id: appLinks
            width: parent.width
            height: appLinkImages.height + appLinkDonate.height + appLinkAAS.height
            anchors.top: border4.bottom
            color: window.white

            AppLink {
                id: appLinkImages
                width: parent.width
                text: qsTr("Screenshots") + l.emptyString
                show: (detailItem["image1"] !== "" || detailItem["image2"] !== "" || detailItem["image3"] !== "" || detailItem["image4"] !== "" || detailItem["image5"] !== "")
                visible: show
                anchors.top: parent.top
                onClicked: detailPage.pageStack.push(Qt.createComponent("ImagePage.qml"), {image1: detailItem["image1"], image2: detailItem["image2"], image3: detailItem["image3"], image4: detailItem["image4"], image5: detailItem["image5"]})
            }

            Rectangle {
                id: border5
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: appLinkImages.bottom
                visible: appLinkImages.visible
            }

            AppLink {
                id: appLinkDonate
                width: parent.width
                text: qsTr("Donate") + l.emptyString
                show: (detailItem["donation"] !== "")
                visible: show
                anchors.top: border5.bottom
                onClicked: symbianFunctions.openUrlInDefaultBrowser(detailItem["donation"])
            }

            Rectangle {
                id: border6
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: appLinkDonate.bottom
                visible: appLinkDonate.visible
            }

            AppLink {
                id: appLinkAAS
                width: parent.width
                text: qsTr("Read on All About Symbian") + l.emptyString
                show: (detailItem["aas"] !== "")
                visible: show
                anchors.top: border6.bottom
                onClicked: symbianFunctions.openUrlInDefaultBrowser(detailItem["aas"])
            }

            Rectangle {
                id: border7
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.top: appLinkAAS.bottom
                visible: appLinkAAS.visible
            }
        }
    }

    ScrollDecorator {
        flickableItem: flickArea
    }

    function openSelectVersion() {
        detailPage.pageStack.push(Qt.createComponent("SelectVersion.qml"), {downloadUrl: detailItem["download"], downloadStoreUrl: detailItem["downloadstore"], downloadUnsignedUrl: detailItem["downloadunsigned"], version: detailItem["version"], versionStore: detailItem["versionstore"], versionUnsigned: detailItem["versionunsigned"], versiondate: Qt.formatDate(detailItem["versiondate"], "d.MM.yy"), versiondateStore: Qt.formatDate(detailItem["versiondatestore"], "d.MM.yy"), versiondateUnsigned: Qt.formatDate(detailItem["versiondateunsigned"], "d.MM.yy"), unsignedNote: detailItem["unsignednote"], installed: detailItem["installed"], sameUid: (detailItem["uid"] === detailItem["uidunsigned"]), download: (appButton.buttonAction == 0 || appButton.buttonAction == 7)})
    }

    InfoBanner {
        id: infoBanner
        text: ""
    }

    Connections {
        target: getList
        onInstallServerFinished: {
            //appDescriptionLabel.text = debug

            if (statusCode === 0) {
                // Successful installation
                updateButton()
            } else {
                if (appButton.buttonAction === 2 || (appButton.buttonAction === 0 && detailItem["downloadunsigned"] != "" && getList.getUseUnsigned())) infoBanner.text = qsTr("Installation failed or was aborted. You have tried to install an unsigned app. Are you sure you have a hacked phone?")
                else infoBanner.text = qsTr("Installation failed or was aborted.")
                infoBanner.open()
                updateButton()
            }
        }
        onDownloadError: {
            if (code == 0) {
                infoBanner.text = qsTr("A connection error appeared while trying to start the download.")
                infoBanner.open()
                updateButton()
            } else if (code == 1) {
                infoBanner.text = qsTr("There is not enough space available on your device to download this app. At least %1 is required.").arg(appVersion.sizeString)
                infoBanner.open()
                updateButton()
            } else {
                infoBanner.text = qsTr("An unknown error appeared while trying to start the download.")
                infoBanner.open()
                updateButton()
            }
        }
    }

    function updateButton() {
        appButton.buttonAction = getList.buttonAction(listIndex)
        appButton.text = (appButton.buttonAction == 0 || appButton.buttonAction == 7) ? (qsTr("Download") + l.emptyString) : (appButton.buttonAction == 1 || appButton.buttonAction == 2 || appButton.buttonAction == 3 || appButton.buttonAction == 5) ? (qsTr("Update") + l.emptyString) : (qsTr("Launch") + l.emptyString)
        downloadProgress.visible = false
    }

    DialogComp {
        id: storeDialog
        text: qsTr("In the following the Nokia Store will be opened where you will be able to buy/download this app. After installation finishes close the Nokia Store and come back to AppList to finish the installation process.") + l.emptyString
        buttonTexts: ["Ok"]
        titleText: "Note"
        displayCheckBox: true
        checkBoxText: qsTr("Don't display again") + l.emptyString

        onButtonClicked: {
            downloadProgress.visible = getList.buttonPressed(appButton.buttonAction)
        }

        onCheckBoxCheckedChanged: {
            getList.setStoreNote(!checkBoxChecked)
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton
    }
}
