import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: speedometer
    width: 600
    height: 600

    property real speed: 0
    property real animatedSpeed: 0
    property var gradient: null

    readonly property real gaugeStartAngle: 165

    onSpeedChanged: {
        speedAnimation.to = speed;
        speedAnimation.start();
    }

    NumberAnimation {
        id: speedAnimation
        target: speedometer
        property: "animatedSpeed"
        duration: 100 // ms
        easing.type: Easing.InOutQuad
        onRunningChanged: {
            if (running) {
                updateTimer.start();
            } else {
                updateTimer.stop();
                foregroundCanvas.requestPaint();
            }
        }
    }

    Timer {
        id: updateTimer
        interval: 16  // 60fps
        repeat: true
        running: false
        onTriggered: foregroundCanvas.requestPaint()
    }

    Canvas {
        id: backgroundCanvas
        anchors.fill: parent
        onPaint: {
            let ctx = getContext("2d");
            ctx.reset();
            ctx.translate(width / 2, height / 2);
            ctx.clearRect(-width / 2, -height / 2, width, height);
        }
    }

    Canvas {
        id: foregroundCanvas
        anchors.fill: parent
        onAvailableChanged: {
            if (available) {
                let ctx = getContext("2d");

                gradient = ctx.createRadialGradient(0, 0, 240, 0, 0, 260);
                gradient.addColorStop(0, "rgba(50, 50, 50, 1)");
                gradient.addColorStop(0.5, "white");
                gradient.addColorStop(1, "rgba(50, 50, 50, 0.7)");

                requestPaint();
            }
        }
        onPaint: {
            let ctx = getContext("2d");
            ctx.reset();
            ctx.translate(width / 2, height / 2);

            drawSpeedGauge(ctx);
        }
    }

    function drawSpeedGauge(ctx) {
        if (animatedSpeed > 0) {
            let gaugeEndAngle = gaugeStartAngle + animatedSpeed * 0.8;

            ctx.strokeStyle = gradient;
            ctx.lineWidth = 25;
            ctx.lineCap = "round";

            ctx.beginPath();
            ctx.arc(0, 0, 250, gaugeStartAngle * Math.PI / 180, gaugeEndAngle * Math.PI / 180);
            ctx.stroke();
        }
    }

    Text {
        id: speedText
        text: Math.round(animatedSpeed).toString()
        font.pixelSize: 70
        font.bold: true
        font.italic: true
        color: "white"
        anchors.centerIn: parent
    }

    Text {
        id: unitText
        text: "cm/s"
        font.pixelSize: 30
        font.italic: true
        font.bold: true
        color: "white"
        anchors.left: speedText.right
        anchors.bottom: speedText.bottom
        anchors.leftMargin: 3
        anchors.bottomMargin: 10
    }
}
