#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include "./logic/logic.h"
#include "./logic/drawingcanvas.h"

QHash<QString, QString> loadEnv(const QString& path = ".env") {
    QHash<QString, QString> values{};

    QFile file(path);

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty() || line.startsWith('#')) continue;

            int eq = line.indexOf("=");
            if (eq == -1) continue;

            QString key = line.left(eq).trimmed();
            QString value = line.mid(eq + 1).trimmed();
            values[key] = value;
        }

        file.close();
    } else qDebug() << "There is no .env file";

    return values;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    qmlRegisterUncreatableType<FSM>("DrawTogether", 1, 0, "FSM", "FSM is not creatable - use only for enums");
    qmlRegisterType<DrawingCanvas>("DrawingLib", 1, 0, "DrawingCanvas");

    auto env = loadEnv();
    QString SERVER_URL = env.contains("SERVER_URL") ? env["SERVER_URL"] : "127.0.0.1",
        SERVER_PORT = env.contains("SERVER_PORT") ? env["SERVER_PORT"] : "5050";

    Logic logic(QUrl(QString("wss://%1:%2").arg(SERVER_URL).arg(SERVER_PORT)));

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
