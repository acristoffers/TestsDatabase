#include "DataBasePrivate.h"

DataBasePrivate::~DataBasePrivate() {
    sqlite3_close(db);
}
