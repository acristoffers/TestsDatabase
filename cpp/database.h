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

#ifndef DATABASE_H
#define DATABASE_H

#include <QtCore>
#include <QtSql>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QString file_path);
    inline bool isValid() { return _db.isOpen(); }
    inline void close() { if (isValid()) _db.close(); }

    void category_add(QString name, int parent);

    // return all immediate sub-categories and questions, as JSON
    QString category_children(int id);
    QStringList category(int id);

    QString category_all();
    void category_update(int id, QString name, int parent);
    void category_merge(int from, int into);
    void category_delete(int id);

    QStringList question(int id);
    int  question_new(QString title, QString reference, int difficulty, QString body, int category);
    void question_update(int id, QString title, QString reference, int difficulty, QString body, int category);
    void question_delete(int id);

    void answer_new(QString body, int question, bool right);
    void answer_delete(int question);
    QString answer_all(int question);

    int test_create(QString title, QString body, QString header);
    QStringList test(int id);
    QString test_all();
    void test_delete(int id);
    QString last_header();

    QStringList questions_for_print(QStringList categories,QStringList min_max_difficulty);

private:
    ~DataBase();
    QSqlDatabase _db;
    QSqlQuery* query;

    void checkDBVersion();
    void initDB();
    void updateDB(int dbversion);

    QList<int> category_tree(int id);

    void init_db_test();
};

#endif // DATABASE_H
