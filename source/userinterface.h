#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QApplication>
#include <QDir>
#include <QObject>
#include <QTranslator>
#include <QWebView>

#define _UI UserInterface::instance()

class DataBase;

class UserInterface : public QObject
{
    Q_OBJECT
public:
    static UserInterface* instance();

    void setDataBase(QString path);

public slots:
    bool checkDatabase();
    void closeDatabase();
    void installTranslation(QString);
    QString jstr(QString);
    QString availableTranslations();
    void openDialogAndChooseFile();
    void openDialogAndCreateFile();
    void pageNavigated(QUrl);

protected:
    UserInterface();

private:
    static UserInterface* _self;
    QWebView _webview;
    QTranslator* translator;
    DataBase* _db;

    void loadIndex();
    
public:
    inline static QString resolved_path(QString path)
    {
#ifdef Q_OS_UNIX
#ifdef Q_OS_MAC
        if (!QDir::isAbsolutePath(path))
            return qApp->applicationDirPath()
                    + QLatin1String("/../Resources/") + path;
#else
        const QString pathInInstallDir = qApp->applicationDirPath()
                + QLatin1String("/../") + path;
        if (pathInInstallDir.contains(QLatin1String("opt"))
                && pathInInstallDir.contains(QLatin1String("bin"))
                && QFileInfo(pathInInstallDir).exists()) {
            return pathInInstallDir;
        }
#endif
#endif
        return qApp->applicationDirPath() + "/" + path;
    }
};

#endif // USERINTERFACE_H
