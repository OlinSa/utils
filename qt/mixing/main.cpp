#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QtQml>

#include "mixing.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Mixing>("an.qt.Mixing",1,0,"Mixing");
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QQmlComponent compontext(&engine, url);

    //
    //    engine.load(url);
    //    if (engine.rootObjects().isEmpty()){
    //        return -1;
    //    }

    // Sets the object to a context property
    Mixing mixing;
    QObject *object = compontext.create();
    object->setProperty("width", 320);
    object->setProperty("height", 240);

    QObject *rect = object->findChild<QObject*>("rect");
    if(rect) {
        rect->setProperty("color", "orange");
    }

    QVariant returnedValue;
    QVariant message = "Hello from c++";
    QMetaObject::invokeMethod(object,"qmlFunction",
                              Q_RETURN_ARG(QVariant,returnedValue),
                              Q_ARG(QVariant,message));
    qDebug() << "returnedValue is " << returnedValue.toString();
    QObject::connect(object, SIGNAL(qmlSignal(QString)),
                     &mixing, SLOT(cppSlot(QString)));

    QQuickView view;
    view.rootContext()->setContextProperty("mixing", &mixing);
    view.rootContext()->setContextProperty("Data", QString("set QML Context"));
    view.setSource(QUrl(QStringLiteral("qrc:/Rectangle.qml")));
    view.show();

    return app.exec();
}
