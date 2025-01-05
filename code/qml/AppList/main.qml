import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

PageStackWindow {
    id: window
    initialPage: MainPage {}
    showStatusBar: true
    showToolBar: true

    property int appId: -1

    property bool invert: false
    property string white: ""
    property string whiteTransparent: ""
    property string lightgrey: ""
    property string grey: ""
    property string black: ""

    property bool aasFeed: true

    Component.onCompleted: {
        if (settings.darkTheme()) {
            invert = true
            white = "black"
            whiteTransparent = "#00000000"
            lightgrey = "grey"
            grey = "lightgrey"
            black = "#FCFCFC"
        } else {
            white = "#FCFCFC"
            whiteTransparent = "#00FFFFFF"
            lightgrey = "lightgrey"
            grey = "grey"
            black = "black"
        }

        aasFeed = settings.aasFeed()

        getList.loadMainList()

        if (launchAppId !== -1) {
            // Open app in AppList
            getList.loadUpdateList(launchAppId)
            appId = launchAppId
        }
    }

    Connections {
        target: getList
        onAppListUpdate: {
            console.debug("appListUpdate QML")
            window.pageStack.push(Qt.createComponent("UpdatePage.qml"))
        }
        onUpdateListChanged: {
            if (appId !== -1) {
                appId = -1
                getList.setCurrentList(4)
                window.pageStack.push (Qt.createComponent("DetailPage.qml"), { "listIndex" : 0, "detailItem": getList._updateList.getModelData(0)})
            }
        }
    }

    Connections {
        target: subscriber
        onAppListIdReceived: {
            if (appListId !== -1) {
                getList.loadUpdateList(appListId)
                appId = appListId
            }
        }
    }

    ToolBarLayout {
        id: toolBarLayout
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: window.pageStack.depth <= 1 ? closeApp() : window.pageStack.pop()
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-home"
            onClicked: if (window.pageStack.depth > 1) window.pageStack.pop(null)
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-list"
            onClicked: window.pageStack.push( Qt.createComponent("CategoryPage.qml") )
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-search"
            onClicked: window.pageStack.push( Qt.createComponent("SearchPage.qml") )
        }

        ToolButton {
            flat: true
            iconSource: "images/contacts.svg"
            onClicked: window.pageStack.push ( Qt.createComponent("PersonalPage.qml") )

            Image {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenterOffset: -parent.height/2
                anchors.horizontalCenterOffset: parent.width/4
                width: parent.width/2
                height: parent.height/2
                source: "images/updateicon.png"
                visible: (getList._updates > 0)

                Text {
                    anchors.centerIn: parent
                    color: "#FCFCFC"
                    font.pixelSize: 16
                    text: getList._updates
                }
            }
        }
    }

    function closeApp() {
        //getList.saveUpdateIdList()
        if (getList.isDownloading()) closeDialog.open()
        else Qt.quit()
    }

    DialogComp {
        id: closeDialog
        text: qsTr("You are currently downloading an app. If you will now close AppList, the download will be aborted and has to be restarted next time.")
        buttonTexts: [qsTr("Close"), qsTr("Cancel")]
        titleText: "Note"

        onButtonClicked: {
            if (index == 0) {
                // Close app
                Qt.quit()
            } else {
                closeDialog.close()
            }
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }
}
