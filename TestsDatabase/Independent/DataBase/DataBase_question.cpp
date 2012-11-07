#include "DataBase.h"
#include "DataBasePrivate.h"
#include "sqlite3.h"

#include <stdio.h>

#define sql_param(x) sqlite3_bind_parameter_index(stmt,x)
#define SQL_ERROR if( sqlite3_errcode(_p->db) != SQLITE_OK ) { printf("Error: %s\n", sqlite3_errmsg(_p->db)); return -1; }

void DataBase::question_delete(int id)
{
    std::string query = "DELETE FROM answers WHERE question=";
    query += IntToStdString(id);
    executeSql(query.c_str());
    
    query = "DELETE FROM questions WHERE id=";
    query += IntToStdString(id);
    executeSql(query.c_str());
}

int DataBase::question_insert(std::string title, std::string reference, int difficulty, std::string body, int category)
{
    int id = 1;
    
    SqlResult r = executeSql("SELECT MAX(id) FROM questions");
    if ( !r.empty() )
        id = SqlInt(r[0]["MAX(id)"]) + 1;
    
    sqlite3_stmt* stmt;
    sqlite3_prepare(_p->db, "INSERT INTO questions VALUES (:id, :title, :body, :difficulty, :reference, :category)", -1, &stmt, 0 ); SQL_ERROR

    sqlite3_bind_int ( stmt, sql_param(":id"), id ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":title"), title.c_str(), title.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":body"), body.c_str(), body.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":difficulty"), difficulty ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":reference"), reference.c_str(), reference.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":category"), category ); SQL_ERROR

    if ( sqlite3_step(stmt) != SQLITE_DONE )
        return -1;
    
    sqlite3_finalize(stmt);
    return id;
}

SqlRow DataBase::question_select(int id)
{
    std::string query = "SELECT * FROM questions WHERE id=";
    query += IntToStdString(id);
    
    SqlResult r = executeSql(query);
    
    if( r.empty() )
        return SqlRow();
    
    return r[0];
}

SqlResult DataBase::question_select_where(std::string where_clause)
{
    std::string query = "SELECT * FROM questions";
    
    if ( !where_clause.empty() ) {
        query += " WHERE ";
        query += where_clause;
    }
    
    return executeSql(query);
}

bool DataBase::question_update(int id, std::string title, std::string reference, int difficulty, std::string body, int category)
{
    std::string query = "UPDATE questions SET title=:title, body=:body, difficulty=:difficulty, reference=:reference, category=:category WHERE id=";
    query += IntToStdString(id);

    sqlite3_stmt* stmt;
    sqlite3_prepare(_p->db, query.c_str(), -1, &stmt, 0 ); SQL_ERROR

    sqlite3_bind_text( stmt, sql_param(":title"), title.c_str(), title.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":body"), body.c_str(), body.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":difficulty"), difficulty ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":reference"), reference.c_str(), reference.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":category"), category ); SQL_ERROR

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    
    sqlite3_finalize(stmt);
    
    return result;
}
