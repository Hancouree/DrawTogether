#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "./logic/logic.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Logic logic;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("logic", &logic);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("DrawTogether", "Main");

    return app.exec();
}
