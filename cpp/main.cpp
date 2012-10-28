/* (c) 2012 Álan Crístoffer */

#include <QtGui>
#include <QtWebKit>

#include "uibridge.h"
#include "database.h"
#include "application.h"

void load_translations()
{
    QTranslator qt_tr;
    qt_tr.load(":/qt_pt.qm");
    qApp->installTranslator(&qt_tr);

    QTranslator qt_help_pt;
    qt_help_pt.load(":/qt_help_pt.qm");
    qApp->installTranslator(&qt_help_pt);
}

int main(int argc, char *argv[])
{
    Application app(argc, argv);

    app.setApplicationName(QObject::tr("Test Database"));
    app.setApplicationVersion("1.0.0");

    app.setWindowIcon(QIcon(":/icon.png"));

    load_translations();

    QWebView viewer;
    UIBridge::instance(viewer.page()->mainFrame());

#ifdef Q_OS_WIN32
    if ( argc > 1 ) _UI->open_db(argv[1]);
#endif

    viewer.page()->mainFrame()->addToJavaScriptWindowObject("cpp", _UI);

    viewer.showMaximized();
    viewer.load(QUrl(UIBridge::resolved_path("html/index.html")));
    viewer.setWindowTitle(QObject::tr("Test Database"));

    _UI->set_language(QLocale::system().uiLanguages().first());

    return app.exec();
}
