import QtQuick 1.1
import com.nokia.symbian 1.1
import QtMobility.feedback 1.1
import QtWebKit 1.0

Rectangle {
    id: newsElement

    // For WebView to fill complete page
    property int topMargin: 0

    // Signalling browser opening change
    signal browserChanged(bool status)
    property bool browserStatus: false

    Component.onCompleted: {
        if (settings.aasFeed()) {
            if (news.updateRunning()) {
                loadingItem.start()
            } else if (newsList.count == 0) {
                loadingItem.start()

                news.fetch();
            }
        }
    }

    color: window.white

    ListView {
        id: newsList
        anchors { top: parent.top; bottom: parent.bottom; left: parent.left; right: parent.right; }
        clip: true

        model: news._model

        delegate: Item {
            id: delegate
            width: parent.width
            height: newsTitle.height + 10

            Rectangle {
                anchors.fill: parent
                color: mouseArea.pressed ? "#4791D9" : window.white

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent

                    onPressed: tf.play()
                    onReleased: tf.play()
                    onClicked: {
                        if (!news.getOpenInBrowser()) {
                            news.fetchHtml(link);
                            //webView.url = "http://www.readability.com/read?url=" + link
                            newsElement.browserStatus = true
                            newsElement.browserChanged(true)
                            flickArea.state = "browser"
                            //progressBar.visible = true
                        } else {
                            //Qt.openUrlExternally(link)
                            symbianFunctions.openUrlInDefaultBrowser(link)
                        }
                    }
                }
            }

            TextEdit {
                id: newsTitle
                readOnly: true
                text: "<h3>" + title + "</h3>"// + description
                width: parent.width - 20
                wrapMode: TextEdit.Wrap
                font.pixelSize: 19
                color: mouseArea.pressed ? window.white : window.black
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 5
            }

            Rectangle {
                id: border
                height: 1
                width: parent.width
                color: window.lightgrey
                visible: (index == 0 || (index == (model.count-1))) ? false : true
            }
        }
    }

    ScrollDecorator {
        flickableItem: newsList
    }

    TextEdit {
        id: noUrlText
        readOnly: true
        wrapMode: TextEdit.Wrap
        width: parent.width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color: window.lightgrey
        font.pixelSize: 29
        opacity: 0.4
        visible: newsList.count > 0 ? false : true

        text: "<div align=\"center\">"+qsTr("Error fetching news feed")+"...</div>" + l.emptyString
    }

    Connections {
        target: news
        onListChanged: {
            loadingItem.stop()
        }
        onHtmlChanged: {
            webView.html = news.getHtml()
        }
    }

    Loading {
        id: loadingItem

        anchors.fill: parent

        text: qsTr("Loading") + "..." + l.emptyString
    }

    Rectangle {
        id: flickArea
        width: parent.width
        anchors.top: parent.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 0
        enabled: false
        color: window.white
        z: 5

        Flickable {
            id: flickable
            anchors.fill: parent
            contentWidth: webView.width
            contentHeight: webView.height
            flickableDirection: Flickable.VerticalFlick
            clip: true

            WebView {
                id: webView
                url: ""
                html: "<!DOCTYPE html>\n<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body style=\"background-color: #FFFFFF; color: #111111;\">\n</body>\n</html>"
                preferredWidth: newsElement.width
            }
        }

        ScrollDecorator {
            flickableItem: flickable
        }

        states: [
            State {
                name: "browser"
                PropertyChanges {
                    target: flickArea
                    enabled: true
                    anchors.topMargin: -newsElement.topMargin
                }
                AnchorChanges {
                    target: flickArea
                    anchors.top: newsElement.top
                }
            }
        ]

        transitions: [
            Transition {
                from: ""
                to: "browser"
                reversible: true
                AnchorAnimation {
                    duration: 150
                }
                PropertyAnimation {
                    properties: "anchors.topMargin,x,y"
                    duration: 150
                }
            }
        ]
    }

    function closeBrowser() {
        webView.stop.trigger()
        webView.html = "<!DOCTYPE html>\n<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body style=\"background-color: #FFFFFF; color: #111111;\">\n</body>\n</html>"
        flickArea.state = ""
        newsElement.browserChanged(false)
        newsElement.browserStatus = false
    }

    ToolBarLayout {
        ToolButton {
            id: centerButton
            flat: true
            iconSource: "toolbar-settings"
            enabled: true
            visible: true

            onClicked: {
                if (news.feedUrl() != "") {
                    textField.text = news.feedUrl()
                    textField.urlBeforeChange = news.feedUrl()
                }
                settingsDialog.open()
            }
        }

        ToolButton {
            id: rightButton
            flat: true
            iconSource: "toolbar-refresh"

            onClicked: {
                if (flickArea.state == "") {
                    loadingItem.start()
                    news.fetch();
                } else if (flickArea.state == "browser") {
                    webView.stop.trigger()
                    webView.html = "<!DOCTYPE html>\n<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>\n<body style=\"background-color: #000000; color: #DDDDDD;\">\n</body>\n</html>"
                    flickArea.state = ""
                }
            }
        }
    }

    ThemeEffect {
        id: tf
        effect: ThemeEffect.BasicButton;
    }
}
