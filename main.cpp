#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "./logic/logic.h"
#include "./logic/drawingcanvas.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterUncreatableType<FSM>("DrawTogether", 1, 0, "FSM", "FSM is not creatable - use only for enums");
    qmlRegisterType<DrawingCanvas>("DrawingLib", 1, 0, "DrawingCanvas");

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
