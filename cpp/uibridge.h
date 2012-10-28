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

#ifndef UIBRIDGE_H
#define UIBRIDGE_H

#include <QtWebKit>
#include <QtGui>
#include "database.h"

#define _UI UIBridge::instance()

class UIBridge : public QObject
{
    Q_OBJECT
public:
    static UIBridge* instance(QWebFrame* frame = 0);

    void show_error(QString title, QString message);

private:
    UIBridge(QObject *parent = 0);
    static UIBridge* _self;
    static QWebFrame* _frame;

    DataBase* _db;

    void htmlTranslations();

    QTranslator* current_lang;

public slots:
    QString get_languages();
    void print(QString type, QString html);

    void set_language(QString lang);

    inline bool hasDataBase() { return _db && _db->isValid(); }
    inline void debug(QString text) { qDebug() << text; }
    inline QString jstr(QString t) { return tr(t.toLocal8Bit()); }
    inline QString jstrdate(QString t) { return QDate::fromString(t, "yyyy-MM-dd").toString(Qt::SystemLocaleLongDate); }

    inline void checkDB() { _frame->evaluateJavaScript("checkDB();"); }
    inline void about_qt() { QMessageBox::aboutQt(0); }

    void choose_and_open_db();
    void create_and_open_db();
    void open_db(QString path);

    QStringList choose_and_encode_image();

    void category_new(QString name, int parent);

    inline QStringList category(int id)
    { if (_db) return _db->category(id); else return QStringList(); }

    inline QString category_children(int id)
    { if (_db) return _db->category_children(id); else return "[]"; }

    inline QString category_all()
    { if (_db) return _db->category_all(); else return "[]"; }

    inline void category_update(int id, QString name, int parent)
    { if (_db) _db->category_update(id,name,parent); }

    inline void category_merge(int from, int to)
    { if (_db) _db->category_merge(from, to); }

    inline void category_delete(int id)
    { if (_db) _db->category_delete(id); }

    inline int  question_new(QString title, QString reference, int difficulty, QString body, int category)
    { if (_db) return _db->question_new(title, reference, difficulty, body, category); else return -2; }

    inline void question_update(int id, QString title, QString reference, int difficulty, QString body, int category)
    { if (_db) _db->question_update(id, title, reference, difficulty, body, category); }

    inline void answer_new(QString body, int question, bool right)
    { if (_db) _db->answer_new(body, question, right); }

    inline void answer_delete(int question)
    { if (_db) _db->answer_delete(question); }

    inline QString answer_all(int question)
    { if (_db) return _db->answer_all(question); else return "[]"; }

    inline QStringList question(int id)
    { if (_db) return _db->question(id); else return QStringList(); }

    inline void question_delete(int id)
    { if (_db) _db->question_delete(id); }

    inline void clear_caches()
    { QWebSettings::clearMemoryCaches(); }

    inline QStringList questions_for_print(QStringList categories,QStringList min_max_difficulty)
    { if(_db) return _db->questions_for_print(categories, min_max_difficulty); else return QStringList(); }

    inline int test_create(QString title, QString body, QString header)
    { if(_db) return _db->test_create(title,body,header); else return -1; }

    inline QStringList test(int id)
    { if(_db) return _db->test(id); else return QStringList(); }

    inline QString last_header()
    { if (_db) return _db->last_header(); else return QString(); }

    inline QString test_all()
    { if (_db) return _db->test_all(); else return QString(); }

    inline void test_delete(int id)
    { if (_db) _db->test_delete(id); }

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

#endif // UIBRIDGE_H
