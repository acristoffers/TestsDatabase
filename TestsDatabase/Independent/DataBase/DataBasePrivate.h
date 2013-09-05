#ifndef DATABASEPRIVATE_H
#define DATABASEPRIVATE_H

#include "sqlite3.h"
#include <string>

class DataBasePrivate
{
public:
    ~DataBasePrivate();

    sqlite3 *db;

    bool        error;
    std::string errorMsg;
};
#endif
