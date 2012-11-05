#include <application.h>
#include <userinterface.h>

#include <QLocale>
#include <QTranslator>

void load_translations()
{
    QTranslator qt_tr;
    qt_tr.load(":/qt_pt.qm");
    qApp->installTranslator(&qt_tr);

    QTranslator qt_help_pt;
    qt_help_pt.load(":/qt_help_pt.qm");
    qApp->installTranslator(&qt_help_pt);
}


int main (int argc, char **argv)
{
    Application app(argc, argv);

    app.setApplicationName(QObject::tr("Tests Database"));
    app.setApplicationVersion("1.0.0");

    app.setWindowIcon(QIcon(":/icon.png"));

    load_translations();

    _UI;

#ifdef Q_OS_WIN32
    if ( argc > 1 ) _UI->setDataBase(argv[1]);
#endif

    _UI->installTranslation(QLocale::system().uiLanguages().first());

    return app.exec();
}
