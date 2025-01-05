import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

Page {
    id: categoryListPage

    tools: toolBarLayout

    property int categoryId: 0

    onStatusChanged: {
        if (status === PageStatus.Activating) getList.setCurrentList(1)
    }

    Component.onCompleted: {
        appList.startLoading()
        getList.loadCategoryList(categoryId,0,true)
    }

    Rectangle {
        color: window.white
        anchors.fill: parent

        Rectangle {
            id: categoryListHeader
            width: parent.width
            height: categoryListHeaderText.height + 11
            color: window.white

            Text {
                id: categoryListHeaderText
                text: l.categoryName(categoryId)
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

            MouseArea {
                anchors.fill: parent
                onPressed: tf.play()
                onReleased: tf.play()
                onClicked: appList.goToIndex(0)
            }
        }

        AppListView {
            id: appList
            anchors { top: categoryListHeader.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }

            listModel: getList._categoryList

            onAtYEndChanged: {
                if (atYEnd && appList.count > 0 && appList.count%25 == 0 && !appList.currentlyLoading) {
                    appList.currentlyLoading = true
                    getList.loadCategoryList(categoryId, appList.count, false)
                }
            }

            onRefreshEvent: {
                getList.loadCategoryList(categoryId,0,true)
            }
        }
    }

    Connections {
        target: getList
        onCategoryListChanged: {
            console.debug("categoryListChanged QML")
            appList.stopLoading()
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton
    }
}
