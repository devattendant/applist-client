import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

Page {
    id: mainPage
    tools: toolBarLayoutMain

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            getList.setCurrentList(0)
        }
    }

    Rectangle {
        id: appListItem
        width: parent.width
        height: window.aasFeed ? parent.height/2 : parent.height
        color: window.white

        Rectangle {
            id: appListHeader
            width: parent.width
            height: appListHeaderText.height + 11
            color: window.white

            Text {
                id: appListHeaderText
                text: qsTr("Latest apps and games") + l.emptyString
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

        AppListView {
            id: appList
            anchors { top: appListHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }

            listModel: getList._mainList

            onRefreshEvent: {
                getList.loadMainList()
            }
        }

        Connections {
            target: getList
            onMainListChanged: {
                console.debug("mainListChanged QML")
                appList.stopLoading()
            }
        }
    }

    Rectangle {
        id: border2
        height: 2
        width: parent.width
        color: window.black
        anchors.top: appListItem.bottom
        anchors.topMargin: -1
    }

    Rectangle {
        id: newsItem
        width: parent.width
        height: window.aasFeed ? parent.height/2 : 0
        anchors.top: border2.bottom
        color: window.white
        visible: window.aasFeed

        Rectangle {
            id: newsHeader
            width: parent.width
            height: newsHeaderText.height + 11
            color: window.white

            Text {
                id: newsHeaderText
                text: qsTr("Latest news from All About Symbian") + l.emptyString
                width: parent.width - 20
                wrapMode: TextEdit.Wrap
                font.pixelSize: 18
                color: window.black
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            Rectangle {
                id: border3
                height: 1
                width: parent.width
                color: window.black
                anchors.bottom: parent.bottom
            }
        }

        NewsElement {
            id: newsElement
            anchors { top: newsHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }
            topMargin: mainPage.height/2 + newsHeader.height
        }
    }

    ToolBarLayout {
        id: toolBarLayoutMain
        ToolButton {
            flat: true
            iconSource: window.pageStack.depth <= 1 ? "images/close_stop.svg" : "toolbar-back"
            onClicked: {
                if (newsElement.browserStatus) {
                    // Browser open, close browser
                    newsElement.closeBrowser()
                } else {
                    // Back
                    window.pageStack.depth <= 1 ? window.closeApp() : window.pageStack.pop()
                }
            }
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-home"
            visible: !newsElement.browserStatus
            enabled: !newsElement.browserStatus
            onClicked: if (window.pageStack.depth > 1) window.pageStack.pop(null)
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-list"
            visible: !newsElement.browserStatus
            enabled: !newsElement.browserStatus
            onClicked: window.pageStack.push( Qt.createComponent("CategoryPage.qml") )
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-search"
            visible: !newsElement.browserStatus
            enabled: !newsElement.browserStatus
            onClicked: window.pageStack.push( Qt.createComponent("SearchPage.qml") )
        }

        ToolButton {
            flat: true
            iconSource: "images/contacts.svg"
            visible: !newsElement.browserStatus
            enabled: !newsElement.browserStatus
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
                    color: "#FFFFFF"
                    font.pixelSize: 16
                    text: getList._updates
                }
            }
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }
}
