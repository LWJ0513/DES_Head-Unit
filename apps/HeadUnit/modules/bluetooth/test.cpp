#include <QCoreApplication>
#include <QBluetoothDeviceDiscovery>
#include <QBluetoothDeviceInfo>
#include <QBluetoothSocket>
#include <QBluetoothUuid>
#include <QDebug>

/**
 * @brief The BluetoothHandler class
 * QBluetoothDeviceDiscoveryAgent - 블루투스 장치 검색
 */

class BluetoothHandler : public QObject {
    Q_OBJECT

public:
    BluetoothHandler(QObject *parent = nullptr) : QObject(parent) {
        discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
                this, &BluetoothHandler::onDeviceDiscovered);
        connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                this, &BluetoothHandler::onScanFinished);

        socket = nullptr;
    }

    void startScan() {
        qDebug() << "Scanning for devices...";
        discoveryAgent->start();
    }

private slots:
    void onDeviceDiscovered(const QBluetoothDeviceInfo &deviceInfo) {
        qDebug() << "Discovered device:" << deviceInfo.name();
        // 여기에서 특정 블루투스 장치를 찾으면 연결 시도
        if (deviceInfo.deviceName() == "Your Bluetooth Speaker") {
            qDebug() << "Found Bluetooth Speaker!";
            connectToDevice(deviceInfo);
        }
    }

    void onScanFinished() {
        qDebug() << "Scan finished.";
    }

    void connectToDevice(const QBluetoothDeviceInfo &deviceInfo) {
        socket = new QBluetoothSocket(QBluetoothSocket::RfcommSocket, this);
        connect(socket, &QBluetoothSocket::connected, this, &BluetoothHandler::onConnected);
        connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothHandler::onDisconnected);
        connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothHandler::onReadyRead);

        qDebug() << "Connecting to" << deviceInfo.name();
        socket->connectToService(deviceInfo.address(), QBluetoothUuid(QBluetoothUuid::SerialPort));
    }

    void onConnected() {
        qDebug() << "Connected to device!";
    }

    void onDisconnected() {
        qDebug() << "Disconnected from device.";
    }

    void onReadyRead() {
        qDebug() << "Data available from the Bluetooth device.";
        QByteArray data = socket->readAll();
        qDebug() << "Received data:" << data;
    }

private:
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;
};

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    BluetoothHandler handler;
    handler.startScan();

    return a.exec();
}

#include "main.moc"
