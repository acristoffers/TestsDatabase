#include "DataBase.h"
#include "DataBasePrivate.h"
#include "sqlite3.h"

#include <stdio.h>

#define sql_param(x) sqlite3_bind_parameter_index(stmt,x)
#define SQL_ERROR if( sqlite3_errcode(_p->db) != SQLITE_OK ) { printf("Error: %s\n", sqlite3_errmsg(_p->db)); return -1; }

void DataBase::test_delete(int id)
{
    std::string query = "DELETE FROM tests WHERE id=";
    query += IntToStdString(id);
    executeSql(query.c_str());
}

int DataBase::test_insert(std::string title, std::string body, std::string header)
{
    int id = 1;
    
    SqlResult r = executeSql("SELECT MAX(id) FROM tests");
    if ( !r.empty() )
        id = SqlInt(r[0]["MAX(id)"]) + 1;
    
    sqlite3_stmt* stmt;
    sqlite3_prepare(_p->db, "INSERT INTO tests VALUES (:id,datetime(),:title,:body)", -1, &stmt, 0 ); SQL_ERROR
    
    unsigned int len;
    char* blob = compress(body, &len);

    sqlite3_bind_int ( stmt, sql_param(":id"), id ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":title"), title.c_str(), title.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_blob( stmt, sql_param(":body"), blob, len, SQLITE_STATIC ); SQL_ERROR

    if ( sqlite3_step(stmt) != SQLITE_DONE )
        return -1;
    
    executeSql("DELETE FROM test_header");
    
    len = 0;
    blob = compress(header, &len);
    
    sqlite3_prepare(_p->db, "INSERT INTO test_header VALUES (:body)", -1, &stmt, 0 ); SQL_ERROR
    
    sqlite3_bind_blob( stmt, sql_param(":body"), blob, len, SQLITE_STATIC ); SQL_ERROR
    
    sqlite3_step(stmt);
    
    sqlite3_finalize(stmt);
    
    free(blob);
    
    return id;
}

SqlRow DataBase::test_select(int id)
{
    std::string query = "SELECT * FROM tests WHERE id=";
    query += IntToStdString(id);
    
    SqlResult r = executeSql(query);
    
    if( r.empty() )
        return SqlRow();
    
    return r[0];
}

SqlResult DataBase::test_select_where(std::string where_clause)
{
    std::string query = "SELECT id, title FROM tests";
    
    if ( !where_clause.empty() ) {
        query += " WHERE ";
        query += where_clause;
    }
    
    return executeSql(query);
}
