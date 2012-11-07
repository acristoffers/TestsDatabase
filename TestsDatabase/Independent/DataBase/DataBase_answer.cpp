#include "DataBase.h"
#include "DataBasePrivate.h"
#include "sqlite3.h"

#include <stdio.h>

#define sql_param(x) sqlite3_bind_parameter_index(stmt,x)
#define SQL_ERROR if( sqlite3_errcode(_p->db) != SQLITE_OK ) { printf("Error: %s\n", sqlite3_errmsg(_p->db)); return -1; }

void DataBase::answer_delete(int question_id)
{
    std::string query = "DELETE FROM answers WHERE question=";
    query += IntToStdString(question_id);
    executeSql(query.c_str());
}

int DataBase::answer_insert(std::string body, int question, bool right)
{
    int id = 1;
    
    SqlResult r = executeSql("SELECT MAX(id) FROM answers");
    if ( !r.empty() )
        id = SqlInt(r[0]["MAX(id)"]) + 1;
    
    sqlite3_stmt* stmt;
    sqlite3_prepare(_p->db, "INSERT INTO answers VALUES (:id, :body, :question, :right)", -1, &stmt, 0 ); SQL_ERROR

    sqlite3_bind_int ( stmt, sql_param(":id"), id ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":body"), body.c_str(), body.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":question"), question ); SQL_ERROR
    sqlite3_bind_int( stmt, sql_param(":right"), (right?1:0) ); SQL_ERROR

    if ( sqlite3_step(stmt) != SQLITE_DONE )
        return -1;
    
    sqlite3_finalize(stmt);
    return id;
}

SqlRow DataBase::answer_select(int question_id)
{
    std::string query = "SELECT * FROM answers WHERE question=";
    query += IntToStdString(question_id);
    
    SqlResult r = executeSql(query);
    
    if( r.empty() )
        return SqlRow();
    
    return r[0];
}

SqlResult DataBase::answer_select_where(std::string where_clause)
{
    std::string query = "SELECT * FROM answers";
    
    if ( !where_clause.empty() ) {
        query += " WHERE ";
        query += where_clause;
    }
    
    return executeSql(query);
}
