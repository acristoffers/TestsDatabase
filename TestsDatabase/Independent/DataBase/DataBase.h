#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <map>
#include <vector>

class DataBasePrivate;

typedef std::map<std::string,std::string> SqlRow;
typedef std::vector<SqlRow> SqlResult;

        int SqlInt (std::string);
       bool SqlBool(std::string);

std::string IntToStdString(int);

class DataBase {
public:
              DataBase(std::string path);
              ~DataBase();
    
    static char* compress(std::string, unsigned int* len);
    static std::string uncompress(char*);
    
         bool isValid() const;
    
         void answer_delete(int question_id);
          int answer_insert(std::string body, int question, bool right);
    SqlResult answer_select(int question_id);
    SqlResult answer_select_where(std::string WHERE_CLAUSE);
    
         void category_delete(int id);
          int category_insert(std::string name, int parent);
       SqlRow category_select(int id);
    SqlResult category_select_where(std::string where_clause);
         bool category_update(int id, std::string name, int parent);
    
         void exportDB(std::string);
    
        void  question_delete(int id);
         int  question_insert(std::string title, std::string reference, int difficulty, std::string body, int category);
       SqlRow question_select(int id);
    SqlResult question_select_where(std::string WHERE_CLAUSE);
         bool question_update(int id, std::string title, std::string reference, int difficulty, std::string body, int category);
    
         void test_delete(int id);
          int test_insert(std::string title, std::string body, std::string header);
       SqlRow test_select(int id);
    SqlResult test_select_where(std::string WHERE_CLAUSE);
    
       SqlRow test_header();

protected:
        bool valid;
    
         void createDatabase();
    SqlResult executeSql(std::string stmt);
         void updateDataBase(int version);
         void verifyVersion();
    
          int getNextVacantID(std::string table);
    
         void recurse_category(int id, std::vector<int>* categories);
    
private:
    DataBasePrivate* _p;
    
         void exportCategory(int thisParent, int foreignParent, DataBase* db);
};

#endif
