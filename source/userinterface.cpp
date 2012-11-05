#include "userinterface.h"

#include "DataBase.h"
#include "FolderCompressor.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDirIterator>
#include <QFileDialog>
#include <QStyle>
#include <QWebFrame>

#include <QDebug>

bool removeDir(const QString &dirName);

UserInterface* UserInterface::_self = 0;

UserInterface* UserInterface::instance()
{
    if (!_self)
        _self = new UserInterface();
    return _self;
}

UserInterface::UserInterface() :
    QObject(0)
{
    _db = 0;
    translator = 0;

    _webview.setWindowIcon(qApp->windowIcon());
    _webview.setWindowTitle(tr(qApp->applicationName().toLocal8Bit().data()));

    _webview.setGeometry(0, 0, 800, 600);

    _webview.page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    connect(&_webview, SIGNAL(linkClicked(QUrl)), this, SLOT(pageNavigated(QUrl)));

    _webview.showMaximized();

    loadIndex();
}

QString UserInterface::availableTranslations()
{
    QString translations_folder = resolved_path("translations");

    QMap<QString,QString> langs;
    QDirIterator qmIt(translations_folder, QStringList() << "*.qm", QDir::Files);
    while(qmIt.hasNext()) {
        qmIt.next();
        QFileInfo finfo = qmIt.fileInfo();
        QTranslator translator;
        translator.load(finfo.baseName(), translations_folder);
        langs.insert(finfo.baseName(), translator.translate("UserInterface", "__LANGUAGE__"));
    }

    QString json = "{";

    QMapIterator<QString,QString> i(langs);
    while (i.hasNext()) {
        i.next();
        if (json != "{") json += ",";
        json += "\"" + i.value() + "\":\"" + i.key() + "\"";
    }

    json += "}";

    return json;
}

bool UserInterface::checkDatabase()
{
    return ( _db && _db->isValid() );
}

void UserInterface::closeDatabase()
{
    delete _db;
    _db = 0;
    _webview.page()->mainFrame()->evaluateJavaScript("App.databaseChanged();");
}

void UserInterface::installTranslation(QString lang)
{
    if ( translator ) {
        QCoreApplication::removeTranslator(translator);
        translator->deleteLater();
    }

    translator = new QTranslator();
    translator->load(lang, resolved_path("translations"));

    QCoreApplication::installTranslator(translator);

    _webview.setWindowTitle(tr("Tests Database"));
}

QString UserInterface::jstr(QString txt)
{
    return tr(txt.toLocal8Bit().data());
}

void UserInterface::loadIndex()
{
    QDir tmpdir = QDir::temp();

    removeDir(tmpdir.filePath("TestsDatabase"));

    tmpdir.mkdir("TestsDatabase");
    tmpdir.cd("TestsDatabase");

    FolderCompressor fc;
    fc.decompressFolder(":/html.qtx", tmpdir.canonicalPath());

    _webview.load(QUrl(tmpdir.filePath("index.html")));

    _webview.page()->mainFrame()->addToJavaScriptWindowObject("AppCore", this);
}

void UserInterface::openDialogAndChooseFile()
{
    QString filename = QFileDialog::getOpenFileName(0, tr("Open file"), QDir::homePath(), "*.qdb");
    if( filename.isEmpty() ) return;

    setDataBase(filename);
}

void UserInterface::openDialogAndCreateFile()
{
    QString filename = QFileDialog::getSaveFileName(0, tr("Create new file"), QDir::homePath(), "*.qdb");
    if( filename.isEmpty() ) return;

    if( QFileInfo(filename).exists() ) {
        QFile f(filename);
        f.open(QFile::ReadWrite);
        f.remove();
    }

    setDataBase(filename);
}

void UserInterface::pageNavigated(QUrl page)
{
    _webview.page()->mainFrame()->evaluateJavaScript("AppNav.navigated('" + page.encodedFragment() + "');");
}

bool removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

void UserInterface::setDataBase(QString path)
{
    if ( _db ) delete _db;
    _db = new DataBase(path.toLocal8Bit().data());
    _webview.page()->mainFrame()->evaluateJavaScript("App.databaseChanged();");
}
