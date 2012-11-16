#include "DataBase.h"
#include "DataBasePrivate.h"
#include "sqlite3.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include <bzlib.h>

#include "AppHandler.h"

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
    delete _p;
}

char* DataBase::compress(std::string str, unsigned int* len)
{
    char* source = (char*) str.data();
    unsigned int srcLen = str.size();
    
    unsigned int destLen = ((unsigned int)(srcLen * 1.01)) + 600;
    char* dest = (char*)malloc(destLen + 8);
    
    dest += 8;
    
    BZ2_bzBuffToBuffCompress(dest, &destLen, source, srcLen, 9, 0, 30);
    
    dest -= 8;
    
    // compressed length
    dest[0] = (destLen & 0xff000000) >> 24;
    dest[1] = (destLen & 0x00ff0000) >> 16;
    dest[2] = (destLen & 0x0000ff00) >> 8;
    dest[3] = (destLen & 0x000000ff);
    // uncompressed length
    dest[4] = (srcLen & 0xff000000) >> 24;
    dest[5] = (srcLen & 0x00ff0000) >> 16;
    dest[6] = (srcLen & 0x0000ff00) >> 8;
    dest[7] = (srcLen & 0x000000ff);
    
    (*len) = destLen + 8;
    
    return dest;
}

std::string DataBase::uncompress(char* data)
{
    unsigned int enSize = (((unsigned char)data[0]) << 24) | (((unsigned char)data[1]) << 16) | (((unsigned char)data[2]) <<  8) | (((unsigned char)data[3]));
    unsigned int deSize = (((unsigned char)data[4]) << 24) | (((unsigned char)data[5]) << 16) | (((unsigned char)data[6]) <<  8) | (((unsigned char)data[7]));
    
    char* dest = (char*)malloc(deSize);
    
    data += 8;
    BZ2_bzBuffToBuffDecompress(dest, &deSize, data, enSize, 0, 0);
    data -= 8;
    
    std::string str(dest, deSize);
    
    free(dest);
    
    return str;
}

bool DataBase::isValid() const
{
    return this->valid;
}

void DataBase::createDatabase()
{
    executeSql("PRAGMA auto_vacuum = 1");

    executeSql("CREATE TABLE IF NOT EXISTS version(version INT)");
    executeSql("CREATE TABLE IF NOT EXISTS categories(id INT PRIMARY KEY, name TEXT, parent INT)");
    executeSql("CREATE TABLE IF NOT EXISTS questions(id INT PRIMARY KEY, title TEXT, body BLOB, difficulty INT, reference TEXT, category INT)");
    executeSql("CREATE TABLE IF NOT EXISTS answers(id INT PRIMARY KEY, body BLOB, question INT, right INT)");
    executeSql("CREATE TABLE IF NOT EXISTS tests(id INT PRIMARY KEY, date DATE, title TEXT, body BLOB)");
    executeSql("CREATE TABLE IF NOT EXISTS test_header(body BLOB)");

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
        if ( p_fields[i] == NULL )
            continue;
        
        if ( std::string(p_col_names[i]) == std::string("body") ) {
            row[p_col_names[i]] = DataBase::uncompress(p_fields[i]);
            continue;
        }
        
        row[p_col_names[i]] = p_fields[i];
    }
    
    if ( row.empty() )
        return 0;
    
    result->push_back(row);
    
    return 0;
}

void DataBase::exportCategory(int thisParent, int foreignParent, DataBase* db)
{
    SqlResult cats = category_select_where("parent=" + IntToStdString(thisParent));
    
    SqlResult::iterator it;
    for( it = cats.begin(); it < cats.end(); it++ ) {
        SqlRow cat = *it;
        int c_id = db->category_insert(cat["name"], foreignParent);
        
        SqlResult questions = question_select_where("category=" + cat["id"]);
        SqlResult::iterator qit;
        for( qit = questions.begin(); qit < questions.end(); qit++ ) {
            SqlRow sq = *qit;
            SqlRow q = question_select(SqlInt(sq["id"]));
            
            int q_id = db->question_insert(q["title"], q["reference"], SqlInt(q["difficulty"]), q["body"], c_id);
            
            SqlResult answers = answer_select(SqlInt(q["id"]));
            SqlResult::iterator ait;
            for ( ait = answers.begin(); ait < answers.end(); ait++ ) {
                SqlRow a = *ait;
                db->answer_insert(a["body"], q_id, SqlInt(a["right"])==1);
            }
        }
        
        exportCategory(SqlInt(cat["id"]), c_id, db);
    }
}

void DataBase::exportDB(std::string file)
{
    DataBase db(file);
    
    // Copy questions on the root;
    SqlResult questions = question_select_where("category=0");
    SqlResult::iterator qit;
    for( qit = questions.begin(); qit < questions.end(); qit++ ) {
        SqlRow q = *qit;
        int q_id = db.question_insert(q["title"], q["reference"], SqlInt(q["difficulty"]), q["body"], 0);
        SqlResult answers = answer_select(SqlInt(q["id"]));
        SqlResult::iterator ait;
        for ( ait = answers.begin(); ait < answers.end(); ait++ ) {
            SqlRow a = *ait;
            db.answer_insert(a["body"], q_id, SqlInt(a["right"])==1);
        }
    }
    
    exportCategory(0, 0, &db);
    
    AppHandler::instance()->openDataBase(file);
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

int DataBase::getNextVacantID(std::string table)
{
    SqlResult r = executeSql("SELECT id FROM " + table);
    std::vector<int> ids;
    
    SqlResult::iterator i;
    for( i = r.begin(); i < r.end(); i++) {
        ids.push_back(SqlInt((*i)["id"]));
    }
    
    for( int j=1;; j++) {
        if(std::find(ids.begin(), ids.end(), j) == ids.end())
            return j;
    }
}
