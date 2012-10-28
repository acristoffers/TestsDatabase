/**********************************************************************************
Copyright (c) 2012 Álan Crístoffer

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************/

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
