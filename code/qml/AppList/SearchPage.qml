import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1

Page {
    id: searchPage

    tools: toolBarLayout

    onStatusChanged: {
        if (status == PageStatus.Activating) getList.setCurrentList(2)
    }

    Rectangle {
        color: window.white
        anchors.fill: parent
    }

    Rectangle {
        id: searchHeader
        width: parent.width
        height: searchHeaderText.height + 11
        anchors.top: parent.top
        color: window.white

        Text {
            id: searchHeaderText
            text: qsTr("Search") + l.emptyString
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

    Item {
        id: search
        width: parent.width
        height: searchField.height + 14
        anchors.top: searchHeader.bottom

        TextField {
            id: searchField
            text: getList.getLastSearchTerm()
            placeholderText: qsTr("Search...") + l.emptyString
            anchors.left: parent.left
            anchors.leftMargin: 7
            anchors.right: searchButton.left
            anchors.rightMargin: 7
            anchors.verticalCenter: parent.verticalCenter
            inputMethodHints: Qt.ImhNoPredictiveText

            Keys.onEnterPressed: {
                closeSoftwareInputPanel()
                searchField.focus = false
                dummy.focus = true

                if (searchField.text !== "") {
                    getList.loadSearchList(searchField.text, 0, true)
                    appList.startLoading()
                }
            }
        }

        //Dummy to remove focus from textfield
        Item { id: dummy }

        Button {
            id: searchButton
            iconSource: "images/search.svg"
            anchors.verticalCenter: searchField.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 7

            onClicked: {
                searchField.focus = false
                dummy.focus = true

                if (searchField.text !== "") {
                    getList.loadSearchList(searchField.text, 0, true)
                    appList.startLoading()
                }
            }
        }
    }

    Rectangle {
        id: border2
        height: 2
        width: parent.width
        color: window.black
        anchors.top: search.bottom
        anchors.topMargin: -1
    }

    AppListView {
        id: appList
        anchors { top: border2.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }

        listModel: getList._searchList

        text: qsTr("Search for apps and games in the AppList database...") + l.emptyString

        onAtYEndChanged: {
            if (atYEnd && appList.count > 0 && appList.count%25 == 0 && !appList.currentlyLoading) {
                appList.currentlyLoading = true
                getList.loadSearchList(searchField.text, 25, false)
            }
        }

        allowPullToRefresh: false
    }

    Connections {
        target: getList
        onSearchListChanged: {
            appList.stopLoading()
            if (appList.count == 0) {
                banner.text = qsTr("No search results found or no internet connection available.")
                banner.open()
            }
        }
    }

    InfoBanner {
        id: banner
    }
}
