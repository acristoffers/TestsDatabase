#include "DataBase.h"
#include "DataBasePrivate.h"
#include "sqlite3.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#define DB_VERSION  1

int         SqlInt (std::string value) { return atoi(value.c_str()); }
bool        SqlBool(std::string value) { return SqlInt(value)==0?false:true; }

std::string IntToStdString(int i)
{
    std::stringstream ss;
    ss << i;
    return ss.str();
}

int selectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

DataBase::DataBase(std::string path)
{
    this->_p      = new DataBasePrivate();
    this->valid   = true;
    
    sqlite3_open(path.c_str(), &_p->db);
    
    if ( _p->db == 0 ) {
        this->valid = true;
    }
    
    verifyVersion();
}

DataBase::~DataBase()
{
    executeSql("VACUUM");
    delete _p;
}

bool DataBase::isValid() const
{
    return this->valid;
}

void DataBase::createDatabase()
{
    //executeSql("PRAGMA auto_vacuum = 1");

    executeSql("CREATE TABLE IF NOT EXISTS version(version INT)");
    executeSql("CREATE TABLE IF NOT EXISTS categories(id INT PRIMARY KEY, name TEXT, parent INT)");
    executeSql("CREATE TABLE IF NOT EXISTS questions(id INT PRIMARY KEY, title TEXT, body TEXT, difficulty INT, reference TEXT, category INT)");
    executeSql("CREATE TABLE IF NOT EXISTS answers(id INT PRIMARY KEY, body TEXT, question INT, right INT)");
    executeSql("CREATE TABLE IF NOT EXISTS tests(id INT PRIMARY KEY, date DATE, title TEXT, html TEXT)");
    executeSql("CREATE TABLE IF NOT EXISTS test_header(header TEXT)");

    std::string query = "INSERT INTO version VALUES(";
    query += IntToStdString(DB_VERSION);
    query += ")";

    executeSql(query);
}

SqlResult DataBase::executeSql(std::string stmt)
{
    _p->error = false;
    
    char *errmsg;
    int   ret;
    SqlResult result;

    ret = sqlite3_exec(_p->db, stmt.c_str(), selectCallback, &result, &errmsg);

    if ( ret != SQLITE_OK ) {
        _p->error = true;
        _p->errorMsg = errmsg;
    }

    return result;
}

int selectCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names)
{
    SqlResult* result = (SqlResult*) p_data;
    SqlRow row;
    
    for(int i=0; i < num_fields; i++) {
        if ( p_fields[i] == NULL ) continue;
        row[p_col_names[i]] = p_fields[i];
    }
    
    if ( row.empty() ) return 0;
    
    result->push_back(row);
    
    return 0;
}

void DataBase::updateDataBase(int version)
{
    switch ( version ) {
        case 0:
        createDatabase();
    }
}

void DataBase::verifyVersion()
{
    SqlResult r = executeSql("SELECT * FROM version");
    
    int version = 0;
    if ( !_p->error && !r.empty() )
        version = SqlInt(r[0]["version"]);
    
    if ( version > DB_VERSION ) {
        _p->error = true;
        _p->errorMsg = "Database file newer version than application supported version";
    } else if ( version < DB_VERSION ) {
        updateDataBase(version);
    }
}

SqlRow DataBase::test_header()
{
    SqlResult r = executeSql("SELECT * FROM test_header");
    if ( r.empty() )
        return SqlRow();
    
    return r[0];
}
