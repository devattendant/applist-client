import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

Page {
    id: updatePage
    tools: toolBarLayout

    property bool listLoaded: false

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            getList.setCurrentList(4)
            if (!listLoaded) {
                updateList.startLoading()
                getList.loadUpdateList(0, true)
                listLoaded = true
            }
        }
    }

    Rectangle {
        id: updateListItem
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        color: window.white

        Rectangle {
            id: updateListHeader
            width: parent.width
            height: updateListHeaderText.height + 11
            color: window.white

            Text {
                id: updateListHeaderText
                text: qsTr("My updates") + l.emptyString
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

        AppListView {
            id: updateList
            anchors { top: updateListHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }

            displayUpdateRect: false

            listModel: getList._updateList

            onAtYEndChanged: {
                if (atYEnd && updateList.count > 0 && updateList.count%25 == 0 && !updateList.currentlyLoading) {
                    updateList.currentlyLoading = true
                    getList.loadUpdateList(updateList.count, false)
                }
            }

            onRefreshEvent: {
                getList.loadUpdateList(0, true)
            }
        }
    }

    Connections {
        target: getList
        onUpdateListChanged: {
            console.debug("updateListChanged QML")
            updateList.stopLoading()
        }
        onUpdateCountChanged: {
            console.debug("updateCountChanged QML")
            if (updatePage.status == PageStatus.Active) {
                updateList.startLoading()
                getList.loadUpdateList(0, true)
                listLoaded = true
            } else {
                // Force reload on next page opening
                listLoaded = false
            }
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }
}
