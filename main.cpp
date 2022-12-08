#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVariant>
#include <QVariantMap>
#include <QDebug>
#include "recorderpagemanagement.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    qmlRegisterSingletonType<RecorderPageManagement>("com.test.RecorderPageManagment", 1, 0, "RecorderPageManagment", &RecorderPageManagement::qmlInstance);

    //RecorderPageManagement *rec=RecorderPageManagement::instance();
    //rec->addPageNumberToUsedList(1,1000,true,false);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);    


    return app.exec();
}
