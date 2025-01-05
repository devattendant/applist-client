import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

Page {
    id: personalPage
    tools: toolBarLayout

    property bool listLoaded: false

    onStatusChanged: {
        if (status == PageStatus.Activating) {
            getList.setCurrentList(3)
            if (!listLoaded) {
                personalList.startLoading()
                getList.loadPersonalList(0, true)
                listLoaded = true
            }
        }
    }

    Rectangle {
        id: aboutItem
        width: parent.width
        height: settingsHeader.height + settingsItem.height + helpItem.height + 1 //11*2
        color: window.white

        Rectangle {
            id: settingsHeader
            width: parent.width
            height: settingsHeaderText.height + 11
            color: window.white

            Text {
                id: settingsHeaderText
                text: qsTr("Settings / About AppList") + l.emptyString
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

        Rectangle {
            id: settingsItem
            anchors.top: settingsHeader.bottom
            height: settingsLabel.height + 34
            color: (settingsMouseArea.pressed) ? "#4791D9" : window.white
            width: parent.width

            Text {
                id: settingsLabel
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                wrapMode: Text.WordWrap

                text: qsTr("Settings") + l.emptyString
                font.pixelSize: 20

                color: (settingsMouseArea.pressed) ? window.white : window.black
            }

            MouseArea {
                id: settingsMouseArea
                anchors.fill: parent

                onPressed: tf.play()
                onReleased: tf.play()
                onClicked: personalPage.pageStack.push( Qt.createComponent("SettingsPage.qml") )
            }
        }

        Rectangle {
            id: border7
            height: 1
            width: parent.width
            color: window.lightgrey
            anchors.top: settingsItem.bottom
        }

        Rectangle {
            id: helpItem
            anchors.top: border7.bottom
            height: label.height + 34
            color: (mouseArea.pressed) ? "#4791D9" : window.white
            width: parent.width

            Text {
                id: label
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                wrapMode: Text.WordWrap

                text: qsTr("Help & Infos about AppList") + l.emptyString
                font.pixelSize: 20

                color: (mouseArea.pressed) ? window.white : window.black
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onPressed: tf.play()
                onReleased: tf.play()
                onClicked: personalPage.pageStack.push( Qt.createComponent("AboutPage.qml") )
            }
        }
    }

    Rectangle {
        id: border2
        height: 2
        width: parent.width
        color: window.black
        anchors.top: aboutItem.bottom
        anchors.topMargin: -1
    }

    Rectangle {
        id: personalListItem
        width: parent.width
        height: parent.height - aboutItem.height
        anchors.top: border2.bottom
        color: window.white

        Rectangle {
            id: personalListHeader
            width: parent.width
            height: personalListHeaderText.height + 11
            color: window.white

            Text {
                id: personalListHeaderText
                text: qsTr("My apps") + l.emptyString
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

        Rectangle {
            id: updateItem
            anchors.top: personalListHeader.bottom
            height: (getList._updates > 0) ? updateLabel.height + 34 : 0
            color: (updateMouseArea.pressed) ? window.white : "#4791D9"
            width: parent.width
            visible: (getList._updates > 0)

            Text {
                id: updateLabel
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.right: parent.right
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width
                wrapMode: Text.WordWrap

                text: qsTr("Updates available") + l.emptyString
                font.pixelSize: 20

                color: (updateMouseArea.pressed) ? window.black : "#FCFCFC"
            }

            Rectangle {
                id: border4
                height: 1
                width: parent.width
                color: window.lightgrey
                anchors.bottom: parent.bottom
            }

            MouseArea {
                id: updateMouseArea
                anchors.fill: parent

                onPressed: tf.play()
                onReleased: tf.play()
                onClicked: personalPage.pageStack.push( Qt.createComponent("UpdatesPage.qml") )
            }
        }

        AppListView {
            id: personalList
            anchors { top: updateItem.bottom; bottom: parent.bottom; left: parent.left; right: parent.right; }

            listModel: getList._personalList

            onAtYEndChanged: {
                if (atYEnd && personalList.count > 0 && personalList.count%25 == 0 && !personalList.currentlyLoading) {
                    personalList.currentlyLoading = true
                    getList.loadPersonalList(personalList.count, false)
                }
            }

            allowPullToRefresh: false
        }
    }

    Connections {
        target: getList
        onPersonalListChanged: {
            console.debug("personalListChanged QML")
            personalList.stopLoading()
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }
}
