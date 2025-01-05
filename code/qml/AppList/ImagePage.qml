import QtQuick 1.1
import com.nokia.symbian 1.1

Page {
    id: imagePage

    tools: toolBarLayoutImage

    property string image1: ""
    property string image2: ""
    property string image3: ""
    property string image4: ""
    property string image5: ""
    property int currentImage: 1

    Loading {
        id: loading

        anchors.fill: parent

        text: qsTr("Loading") + "..." + l.emptyString
    }

    Image {
        id: image
        anchors.fill: parent

        source: image1

        asynchronous: true
        smooth: true
        fillMode: Image.PreserveAspectFit

        onStatusChanged: {
            if (status == Image.Loading) loading.start()
            else loading.stop()
        }
    }

    ToolBarLayout {
        id: toolBarLayoutImage
        ToolButton {
            flat: true
            iconSource: "toolbar-back"
            onClicked: window.pageStack.depth <= 1 ? Qt.quit() : window.pageStack.pop()
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-previous"
            visible: (image2 !== "" || image3 !== "" || image4 !== "" || image5 !== "")
            enabled: (image2 !== "" || image3 !== "" || image4 !== "" || image5 !== "")
            onClicked: {
                if (currentImage == 1) {
                    if (image5 !== "") {
                        image.source = image5
                        currentImage = 5
                    } else if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    } else if (image3 !== "") {
                        image.source = image3
                        currentImage = 3
                    } else {
                        image.source = image2
                        currentImage = 2
                    }
                } else if (currentImage == 2) {
                    if (image1 !== "") {
                        image.source = image1
                        currentImage = 1
                    } else if (image5 !== "") {
                        image.source = image5
                        currentImage = 5
                    } else if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    } else {
                        image.source = image3
                        currentImage = 3
                    }
                } else if (currentImage == 3) {
                    if (image2 !== "") {
                        image.source = image2
                        currentImage = 2
                    } else if (image1 !== "") {
                        image.source = image1
                        currentImage = 1
                    } else if (image5 !== "") {
                        image.source = image5
                        currentImage = 5
                    } else {
                        image.source = image4
                        currentImage = 4
                    }
                } else if (currentImage == 4) {
                    if (image3 !== "") {
                        image.source = image3
                        currentImage = 3
                    } else if (image2 !== "") {
                        image.source = image2
                        currentImage = 2
                    } else if (image1 !== "") {
                        image.source = image1
                        currentImage = 1
                    } else {
                        image.source = image5
                        currentImage = 5
                    }
                } else {
                    if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    } else if (image3 !== "") {
                        image.source = image3
                        currentImage = 3
                    } else if (image2 !== "") {
                        image.source = image2
                        currentImage = 2
                    } else {
                        image.source = image1
                        currentImage = 1
                    }
                }
            }
        }

        ToolButton {
            flat: true
            iconSource: "toolbar-next"
            visible: (image2 !== "" || image3 !== "" || image4 !== "" || image5 !== "")
            enabled: (image2 !== "" || image3 !== "" || image4 !== "" || image5 !== "")
            onClicked: {
                if (currentImage == 1) {
                    if (image2 !== "") {
                        image.source = image2
                        currentImage = 2
                    } else if (image3 !== ""){
                        image.source = image3
                        currentImage = 3
                    } else if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    } else {
                        image.source = image5
                        currentImage = 5
                    }
                } else if (currentImage == 2) {
                    if (image3 !== "") {
                        image.source = image3
                        currentImage = 3
                    } else if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    } else if (image5 !== "") {
                        image.source = image5
                        currentImage = 5
                    } else {
                        image.source = image1
                        currentImage = 1
                    }
                } else if (currentImage == 3) {
                    if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    } else if (image5 !== "") {
                        image.source = image5
                        currentImage = 5
                    } else if (image1 !== ""){
                        image.source = image1
                        currentImage = 1
                    } else {
                        image.source = image2
                        currentImage = 2
                    }
                } else if (currentImage == 4) {
                    if (image5 !== "") {
                        image.source = image5
                        currentImage = 5
                    } else if (image1 !== "") {
                        image.source = image1
                        currentImage = 1
                    } else if (image2 !== ""){
                        image.source = image2
                        currentImage = 2
                    } else {
                        image.source = image3
                        currentImage = 3
                    }
                } else {
                    if (image1 !== "") {
                        image.source = image1
                        currentImage = 1
                    } else if (image2 !== ""){
                        image.source = image2
                        currentImage = 2
                    } else if (image3 !== "") {
                        image.source = image3
                        currentImage = 3
                    } else if (image4 !== "") {
                        image.source = image4
                        currentImage = 4
                    }
                }
            }
        }

        ToolButton {
            visible: false
            enabled: false
        }
    }
}
