#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQmlContext>

#include "modules/spotify/spotify.h"
#include "shared/utils/envmanager.h"
#include "shared/utils/someip.h"
#include "../ServiceManager/src/server.hpp"

std::shared_ptr<vsomeip::application> app;
std::mutex mutex;
std::condition_variable condition;

void init_vSOMEIP() {
    app = vsomeip::runtime::get()->create_application("gear");
    app->init();
    app->register_availability_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, on_availability);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    app->request_service(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID);
    app->register_message_handler(VEHICLE_SERVICE_ID, GEAR_INSTANCE_ID, JOY_GEAR_RESPONSE_MID, on_message);
    std::thread sender(run);
    app->start();
}

int main(int argc, char *argv[])
{
    init_vSOMEIP();

    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QGuiApplication app(argc, argv);

    // todo fix the location
    EnvManager::instance().loadEnvFile("/home/wonjeong/head-unit/apps/HeadUnit/.env");


    // Register the Spotify class to be used in QML
    // qmlRegisterType<Spotify>("com.spotify", 1, 0, "Spotify");


    QQmlApplicationEngine engine;

    Spotify spotify;
    engine.rootContext()->setContextProperty("spotify", &spotify);

    SomeIP someIP;
    engine.rootContext()->setContextProperty("someIP", &someIP);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    // engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    engine.loadFromModule("HeadUnit", "Main");


    qDebug() << "Head Unit launched";

    return app.exec();
}
