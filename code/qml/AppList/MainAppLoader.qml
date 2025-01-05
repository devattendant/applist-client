import QtQuick 1.1

Item {
    Loader {
        id: splashScreenLoader // ID needed for so we can later delete the item
        width: parent.width
        height: parent.height
        source: Qt.resolvedUrl("SplashScreen.qml")
    }

    Loader {
        id: mainLoader
        width: parent.width
        height: parent.height
    }

    Timer {
        id: firstPhaseTimer
        interval: 900
        running: true
        repeat: false

        onTriggered: {
            if (!mainLoader.Loading) {
                // Start to load the main application
                mainLoader.source = Qt.resolvedUrl("main.qml");
                secondPhaseTimer.start();
            }
        }
    }

    Timer {
        id: secondPhaseTimer
        property int phase: 0
        interval: 1500
        running: false
        repeat: true

        onTriggered: {
            if (phase == 0) {
                // Hide the splash screen
                if (splashScreenLoader.item) {
                    splashScreenLoader.item.opacity = 0;
                }

                phase += 1;
            }
            else {
                // Delete the splash screen.
                //
                // By setting the source property to an empty string destroys
                // the loaded item.
                splashScreenLoader.source = "";
                secondPhaseTimer.stop();
            }
        }
    }
}
