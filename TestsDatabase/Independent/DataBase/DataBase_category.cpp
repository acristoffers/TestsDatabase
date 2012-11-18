#include "DataBase.h"
#include "DataBasePrivate.h"
#include "sqlite3.h"

#include <stdio.h>

#define sql_param(x) sqlite3_bind_parameter_index(stmt,x)
#define SQL_ERROR if( sqlite3_errcode(_p->db) != SQLITE_OK ) { printf("Error: %s\n", sqlite3_errmsg(_p->db)); return -1; }

void DataBase::recurse_category(int id, std::vector<int> *categories)
{
    SqlResult::iterator it;
    
    SqlResult sub_categories = category_select_where("parent=" + IntToStdString(id));
    for ( it = sub_categories.begin(); it < sub_categories.end(); it++ ) {
        SqlRow cat = *it;
        categories->push_back(SqlInt(cat["id"]));
        recurse_category(SqlInt(cat["id"]), categories);
    }
}

void DataBase::category_delete(int id)
{
    std::vector<int> categories;
    
    recurse_category(id, &categories);
    categories.push_back(id);
    
    std::vector<int>::iterator it;
    
    std::string cids = "(";
    for ( it = categories.begin(); it < categories.end(); it++ ) {
        if ( it != categories.begin() ) cids += ",";
        cids += IntToStdString(*it);
    }
    cids += ")";
    
    SqlResult qr = question_select_where("category IN " + cids);
    std::string qids = "(";
    
    SqlResult::iterator rit;
    for (rit = qr.begin(); rit < qr.end(); rit++) {
        SqlRow q = *rit;
        if ( rit != qr.begin() ) qids += ",";
        qids += q["id"];
    }
    
    qids += ")";
    
    std::string
    query = "DELETE FROM categories WHERE id IN " + cids;
    executeSql(query.c_str());
    
    query = "DELETE FROM answers WHERE question IN " + qids;
    executeSql(query.c_str());
    
    query = "DELETE FROM questions WHERE id IN " + qids;
    executeSql(query.c_str());
}

int DataBase::category_insert(std::string name, int parent)
{
    int id = getNextVacantID("categories");
    
    sqlite3_stmt* stmt;
    sqlite3_prepare(_p->db, "INSERT INTO categories VALUES (:id, :name, :parent)", -1, &stmt, 0 ); SQL_ERROR

    sqlite3_bind_int ( stmt, sql_param(":id"), id ); SQL_ERROR
    sqlite3_bind_text( stmt, sql_param(":name"), name.c_str(), name.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":parent"), parent ); SQL_ERROR

    if ( sqlite3_step(stmt) != SQLITE_DONE )
        return -1;
    
    sqlite3_finalize(stmt);
    return id;
}

SqlRow DataBase::category_select(int id)
{
    std::string query = "SELECT * FROM categories WHERE id=";
    query += IntToStdString(id);
    
    SqlResult r = executeSql(query);
    
    if( r.empty() )
        return SqlRow();
    
    return r[0];
}

SqlResult DataBase::category_select_where(std::string where_clause)
{
    std::string query = "SELECT * FROM categories";
    
    if ( !where_clause.empty() ) {
        query += " WHERE ";
        query += where_clause;
    }
    
    return executeSql(query);
}

bool DataBase::category_update(int id, std::string name, int parent)
{
    std::string query = "UPDATE categories SET name=:name, parent=:parent WHERE id=";
    query += IntToStdString(id);

    sqlite3_stmt* stmt;
    sqlite3_prepare(_p->db, query.c_str(), -1, &stmt, 0 ); SQL_ERROR

    sqlite3_bind_text( stmt, sql_param(":name"), name.c_str(), name.length(), SQLITE_STATIC ); SQL_ERROR
    sqlite3_bind_int ( stmt, sql_param(":parent"), parent ); SQL_ERROR

    bool result = (sqlite3_step(stmt) == SQLITE_DONE);
    
    sqlite3_finalize(stmt);
    
    return result;
}
