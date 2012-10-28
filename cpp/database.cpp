/* (c) 2012 Álan Crístoffer */

#include "database.h"
#include "uibridge.h"
#include <QTranslator>

#define DB_VERSION  1

void DataBase::init_db_test()
{
    for (int i=0; i<100; i++) {
        int id = question_new("Question "+QString::number(i), "",
                     (qrand() % ((10 + 1) - 1) + 1), "Question "+QString::number(i)+" <br />Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", 0);

        answer_new("Question "+QString::number(id)+" Answer "+QString::number(0), id, true);
        for (int j=1;j<4;j++) {
            answer_new("Question "+QString::number(id)+" Answer "+QString::number(j), id, false);
        }
    }
}

DataBase::DataBase(QString file_path)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(file_path);

    if( !_db.open() ) {
        _UI->show_error(tr("Database error"), tr("Could not open specified file."));
        return;
    }

    query = new QSqlQuery(_db);

    checkDBVersion();
}

void DataBase::initDB()
{
    /*
        Makes SQLite shrink the file after a DELETE
        It works like a defragmentor too, so may slow down things in large databases.
    */
    query->exec("PRAGMA auto_vacuum = 1");

    query->exec("CREATE TABLE IF NOT EXISTS version(version INT)");
    query->exec("CREATE TABLE IF NOT EXISTS categories(id INT PRIMARY KEY, name TEXT, parent INT)");
    query->exec("CREATE TABLE IF NOT EXISTS questions(id INT PRIMARY KEY, title TEXT, body TEXT, difficulty INT, reference TEXT, category INT)");
    query->exec("CREATE TABLE IF NOT EXISTS answers(id INT PRIMARY KEY, body TEXT, question INT, right INT)");
    query->exec("CREATE TABLE IF NOT EXISTS tests(id INT PRIMARY KEY, date DATE, title TEXT, html TEXT)");
    query->exec("CREATE TABLE IF NOT EXISTS test_header(header TEXT)");
    query->exec(QString()+"INSERT INTO version VALUES(" + QString::number(DB_VERSION) + ")");

    init_db_test();
}

void DataBase::updateDB(int dbversion)
{
    qDebug() << "updating db...";

    //switch case withouth break; to get it to the latest version
}

void DataBase::checkDBVersion()
{
    query->exec("SELECT * FROM version");

    if( !query->next() ) {
        initDB();
        return;
    }

    int version = query->value(0).toInt();

    if ( version > DB_VERSION ) {
        _UI->show_error(tr("Out of date app"), tr("Can't open the file because the application is older than the database. Check for application updates."));
    } else if ( version < DB_VERSION ) {
        updateDB(version);
    }
}

DataBase::~DataBase()
{
    _db.close();
}

void DataBase::category_add(QString name, int parent)
{
    query->exec("SELECT MAX(id) FROM categories");
    query->next();
    int id = query->value(0).toInt() + 1;
    query->exec(QString() + "INSERT INTO categories VALUES (" + QString::number(id) + ", \"" + name + "\"," + QString::number(parent) + ")");
}

QString DataBase::category_children(int id)
{
    if( !query ) return "[]";
    QString json = "[";

    query->exec(QString() + "SELECT * FROM categories WHERE parent=" + QString::number(id));
    while(query->next()) {
        if ( json != "[" ) json += ",";
        int id = query->value(query->record().indexOf("id")).toInt();
        QString name = query->value(query->record().indexOf("name")).toString();
        json += QString() + "{\"id\":\"" + QString::number(id) + "\",\"name\":\"" + name + "\",\"kind\":\"category\"}";
    }

    query->exec(QString() + "SELECT * FROM questions WHERE category=" + QString::number(id));
    while(query->next()) {
        if ( json != "[" ) json += ",";
        int id = query->value(query->record().indexOf("id")).toInt();
        QString name = query->value(query->record().indexOf("title")).toString();
        json += QString() + "{\"id\":\"" + QString::number(id) + "\",\"name\":\"" + name + "\",\"kind\":\"question\"}";
    }

    json += "]";
    return json;
}

QStringList DataBase::category(int id)
{
    if (!query) return QStringList();
    if ( id == 0) {
        QStringList list;
        list << "0";
        list << "Root";
        list << "0";
        return list;
    }
    query->exec(QString() + "SELECT * FROM categories WHERE id=" + QString::number(id));
    query->next();
    QStringList list;
    list << query->value(0).toString();
    list << query->value(1).toString();
    list << query->value(2).toString();
    return list;
}

QString DataBase::category_all()
{
    QString c = "[";
    query->exec("SELECT * FROM categories");
    while(query->next()) {
        if( c != "[" ) c += ",";
        c += QString() + "{\"id\":\"" + query->value(0).toString() + "\",\"name\":\"" + query->value(1).toString() + "\",\"parent\":\"" + query->value(2).toString() + "\"}";
    }
    c += "]";
    return c;
}

void DataBase::category_update(int id, QString name, int parent)
{
    query->exec(QString() + "UPDATE categories SET name=\"" + name + "\", parent=" + QString::number(parent) + " WHERE id=" + QString::number(id));
}

void DataBase::category_merge(int from, int into)
{
    query->exec(QString() + "SELECT id FROM categories WHERE parent=" + QString::number(from));
    QString ids;
    while(query->next()) {
        if( !ids.isEmpty() ) ids += ",";
        ids += query->value(0).toString();
    }
    query->exec(QString("UPDATE categories SET parent=:id WHERE id IN (?)").replace("?", ids).replace(":id", QString::number(into)));
    query->exec(QString("UPDATE questions  SET category=:into WHERE category=:from").replace(":into", QString::number(into)).replace(":from", QString::number(from)));
    query->exec(QString("DELETE FROM categories WHERE id=") + QString::number(from));
}

QList<int> DataBase::category_tree(int id)
{
    QList<int> ids;

    query->exec(QString() + "SELECT id FROM categories WHERE parent=" + QString::number(id));
    while(query->next()) {
        int id = query->value(0).toInt();
        ids << id;
    }

    foreach(int id, ids)
        ids << category_tree(id);

    return ids;
}

void DataBase::category_delete(int id)
{
    QList<int> ids_to_delete = category_tree(id);
    ids_to_delete << id;

    QString str_ids;
    foreach(int id, ids_to_delete) {
        if (!str_ids.isEmpty()) str_ids += ",";
        str_ids += QString::number(id);
    }

    query->exec(QString("DELETE FROM categories WHERE id IN (?)").replace("?", str_ids));
    query->exec(QString("SELECT id FROM questions WHERE category IN (?)").replace("?", str_ids));

    ids_to_delete.clear();
    while(query->next()) {
        ids_to_delete << query->value(0).toInt();
    }

    foreach (int id, ids_to_delete)
        question_delete(id);
}

int DataBase::question_new(QString title, QString reference, int difficulty, QString body, int category)
{
    query->exec("SELECT MAX(id) FROM questions");
    query->next();
    int id = query->value(0).toInt() + 1;

    query->prepare("INSERT INTO questions VALUES (:id, :title, :body, :difficulty, :reference, :category)");
    query->bindValue("id", id);
    query->bindValue("title", title);
    query->bindValue("body", qCompress(body.toUtf8(),9));
    query->bindValue("difficulty", difficulty);
    query->bindValue("reference", reference);
    query->bindValue("category", category);
    query->exec();

    return id;
}

void DataBase::question_update(int id, QString title, QString reference, int difficulty, QString body, int category)
{
    query->prepare("UPDATE questions SET title=:title, body=:body, difficulty=:difficulty, reference=:reference, category=:category WHERE id=" + QString::number(id));
    query->bindValue("title", title);
    query->bindValue("body", qCompress(body.toUtf8(), 9));
    query->bindValue("difficulty", difficulty);
    query->bindValue("reference", reference);
    query->bindValue("category", category);
    query->exec();
}

void DataBase::answer_new(QString body, int question, bool right)
{
    query->exec("SELECT MAX(id) FROM answers");
    query->next();
    int id = query->value(0).toInt() + 1;

    query->prepare("INSERT INTO answers VALUES (:id, :body, :question, :right)");
    query->bindValue("id", id);
    query->bindValue("body", body);
    query->bindValue("question", question);
    query->bindValue("right", (right?1:0) );
    query->exec();
}

void DataBase::answer_delete(int question)
{
    query->exec(QString("DELETE FROM answers WHERE question=") + QString::number(question));
}

QString DataBase::answer_all(int question)
{
    query->exec(QString("SELECT * FROM answers WHERE question=") + QString::number(question));
    QString json = "[";

    while (query->next()) {
        if ( json != "[" ) json += ",";
        json += "{\"id\":\"" + query->value(0).toString() + "\",\"body\":\"" + query->value(1).toString() + "\",\"right\":" + (query->value(3).toInt()==1?"true":"false") + "}";
    }

    json += "]";
    return json;
}

QStringList DataBase::question(int id)
{
    query->exec(QString("SELECT * FROM questions WHERE id=") + QString::number(id));
    query->next();

    QStringList l;

    for ( int i = 0; i < 6; i++) {
        if (query->record().fieldName(i) == "body") {
            l << QString(qUncompress(query->value(i).toByteArray()));
        } else {
            l << query->value(i).toString();
        }
    }

    return l;
}

void DataBase::question_delete(int id)
{
    query->exec(QString("DELETE FROM questions WHERE id=") + QString::number(id));
    query->exec(QString("DELETE FROM answers WHERE question=") + QString::number(id));
}

QStringList DataBase::questions_for_print(QStringList categories, QStringList min_max_difficulty)
{
    QString sql = QString("SELECT id FROM questions WHERE category IN (:cats) AND difficulty BETWEEN :min AND :max")
            .replace(":min",min_max_difficulty[0])
            .replace(":max",min_max_difficulty[1])
            .replace(":cats", categories.join(","));

    query->exec(sql);

    QStringList ids;
    while(query->next())
        ids.append(query->value(0).toString());

    return ids;
}

int DataBase::test_create(QString title, QString body, QString header)
{
    query->exec("SELECT MAX(id) FROM tests");
    query->next();
    int id = query->value(0).toInt() + 1;

    query->prepare("INSERT INTO tests VALUES (:id,:date,:title,:body)");
    query->bindValue("id", id);
    query->bindValue("date", QDate::currentDate());
    query->bindValue(":title", title);
    query->bindValue("body", qCompress(body.toUtf8(),9));
    query->exec();

    query->exec("DELETE FROM test_header");
    query->prepare("INSERT INTO test_header VALUES(:header)");
    query->bindValue("header",header);
    query->exec();

    return id;
}

QStringList DataBase::test(int id)
{
    query->exec("SELECT * FROM tests WHERE id="+QString::number(id));
    query->next();

    QStringList list;
    list << query->value(1).toString();
    list << query->value(2).toString();
    list << QString(qUncompress(query->value(3).toByteArray()));

    return list;
}

QString DataBase::last_header()
{
    query->exec("SELECT * FROM test_header");

    QString header;

    if (query->next())
        header = query->value(0).toString();

    return header;
}

QString DataBase::test_all()
{
    QString json = "[";

    query->exec("SELECT id,title FROM tests");

    while( query->next() ) {
        if ( json != "[" ) json += ",";
        json += "[\"" + query->value(0).toString() + "\",\"" + query->value(1).toString() + "\"]";
    }

    json += "]";
    return json;
}

void DataBase::test_delete(int id)
{
    query->exec("DELETE FROM tests WHERE id="+QString::number(id));
}
