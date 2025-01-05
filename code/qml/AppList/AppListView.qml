import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1

ListView {
    id: appList
    anchors { top: parent.top; bottom: parent.bottom; left: parent.left; right: parent.right; }
    clip: true
    z: 2

    property alias listModel: appList.model
    property alias text: placeholderText.text
    property bool displayUpdateRect: true
    property bool allowLoadingMore: true
    property bool currentlyLoading: false //Verhindern von multiple load more elements requests

    property bool allowPullToRefresh: true
    property bool platformInverted: true
    property int latency: 600
    property int rotationThreshold: 135
    property string pullMessageString: "Pull and hold to refresh..."
    property string releaseMessageString: "Release to refresh..."
    signal refreshEvent()

    delegate: Item {
        id: delegate
        width: parent.width
        height: appIcon.height + 11

        Rectangle {
            anchors.fill: parent
            color: mouseArea.pressed ? "#4791D9" : window.white //(index%2==0) ? window.black : "#292929"

            MouseArea {
                id: mouseArea
                anchors.fill: parent

                onPressed: tf.play()
                onReleased: tf.play()
                onClicked: {
                    window.pageStack.push (Qt.createComponent("DetailPage.qml"), { "listIndex" : index, "detailItem": listModel.getModelData(index)})
                }
            }
        }

        Image {
            id: appIcon
            height: 80
            width: 80
            asynchronous: true
            source: icon
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.verticalCenter: parent.verticalCenter
        }

        Label {
            id: appTitle
            text: name
            width: parent.width - 15 - appIcon.width
            font.pixelSize: 28
            color: mouseArea.pressed ? window.white : window.black
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.left: appIcon.right
            anchors.leftMargin: 5
            elide: Text.ElideRight
        }

        Label {
            id: appVersion
            //text: (version !== "") ? version : (versionunsigned !== "") ? versionunsigned : versionstore
            text: l.categoryName(category)
            width: parent.width - 15 - appIcon.width
            wrapMode: TextEdit.Wrap
            font.pixelSize: 20
            color: window.grey
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 11
            anchors.left: appIcon.right
            anchors.leftMargin: 5
        }

        Rectangle {
            id: updateRect
            color: "#4791D9"
            height: parent.height
            width: 10
            anchors.left: parent.left
            visible: ((updateAvailable === "true") && displayUpdateRect)
        }

        Rectangle {
            id: border
            height: 1
            width: parent.width
            anchors.bottom: parent.bottom
            color: window.lightgrey
            visible: ((index < (appList.count-1)) || ((index+1)%25 == 0 && appList.allowLoadingMore)) ? true : false
        }
    }

    footer: Rectangle {
        id: footerRect
        height: (appList.count%25 == 0 && appList.allowLoadingMore && appList.count > 0) ? 91 : 0 //91 = appIcon.height+11
        width: appList.width
        color: window.white

        BusyIndicator {
            id: footerIndicator
            height: parent.height/2
            width: parent.height/2
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            running: footerRect.height > 0
        }
    }

    TextEdit {
        id: placeholderText
        readOnly: true
        wrapMode: TextEdit.Wrap
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color: window.grey
        font.pixelSize: 29
        opacity: 0.4
        visible: appList.count > 0 ? false : true
        horizontalAlignment: TextEdit.AlignHCenter

        text: ""
    }


    Loading {
        id: loading

        anchors.fill: parent

        text: qsTr("Loading") + "..." + l.emptyString
    }

    function startLoading() {
        loading.start()
        currentlyLoading = true
    }
    function stopLoading() {
        loading.stop()
        currentlyLoading = false
    }

    function goToIndex(idx) {
        var pos = appList.contentY;
        var destPos;
        appList.positionViewAtIndex(idx, ListView.Beginning);
        destPos = appList.contentY;
        anim.from = pos;
        anim.to = destPos;
        anim.running = true;
    }
    NumberAnimation { id: anim; target: appList; property: "contentY"; duration: 500 }

    ScrollDecorator {
        flickableItem: appList
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }


    // Pull to refresh
    Item {
            property bool __puller : false

        visible: appList.allowPullToRefresh
        enabled: appList.allowPullToRefresh

            id: pull
            width: parent.width
            opacity: -pullImage.rotation / appList.rotationThreshold
            y: -(appList.contentY + pullImage.height + labelRow.spacing)

            Row {
                id: labelRow
                anchors.left: parent.left
                anchors.leftMargin: spacing
                spacing: pullImage.width / 2
                width: pullImage.width + pullLabel.width + spacing

                Image {
                    id: pullImage
                    smooth: true
                    source: privateStyle.toolBarIconPath("toolbar-refresh", appList.platformInverted)
                    rotation: 2 * 360 * appList.contentY / appList.height
                    onRotationChanged: {
                        if (pullImage.rotation < -appList.rotationThreshold){
                            if (!pullTimer.running && !pull.__puller)
                                pullTimer.restart()
                        }
                        else if (pullImage.rotation > -appList.rotationThreshold){
                            if (!pullTimer.running && pull.__puller)
                                pullTimer.restart()
                        }
                    }

                    Timer{
                        id: pullTimer
                        interval: appList.latency

                        onTriggered: {
                            if(pullImage.rotation < -appList.rotationThreshold)
                                pull.__puller = true
                            else
                                pull.__puller = false
                        }
                    }
                }

                Label {
                    id: pullLabel
                    platformInverted: appList.platformInverted
                    text: {
                        if (pull.__puller)
                            return appList.releaseMessageString

                        return appList.pullMessageString
                    }
                }
            }
        }

        onMovementEnded: {
            if (appList.allowPullToRefresh && pull.__puller) {
                console.log("pull to refresh")
                appList.startLoading()
                appList.refreshEvent()
            }

            pull.__puller = false
            pullTimer.stop()
        }
}
