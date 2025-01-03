import QtQuick

Window {
    id: mainWindow
    visible: true
    width: 1280
    height: 400
    title: qsTr("Instrument Cluster")

    Component.onCompleted: {
        if (Screen.width > width && Screen.height > height) {
            mainWindow.visibility = Window.Windowed;
        } else {
            mainWindow.visibility = Window.FullScreen;
        }
    }

    Image {
        anchors.fill: parent
        source: "qrc:/images/background.png"
    }

    Speedometer {
        id: speedometer
        anchors.centerIn: parent
        anchors.verticalCenterOffset: 100
        speed: 183
    }

    Time {
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 25
        anchors.rightMargin: 60
    }

    Battery {
        id: battery
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 20
        anchors.leftMargin: 90
        isCharging: true
        batteryPercentage: 10
    }

    Gear {
        id: gear
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 50
        currentGear: "N"
    }
}
