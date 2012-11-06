/* (c) 2012 Álan Crístoffer */

#ifndef APPHANDLER_H
#define APPHANDLER_H

#include <string>

#include "include/cef_v8.h"

#include "DataBase.h"

class ClientHandler;

class AppHandler {
public:
    ClientHandler* clientHandler;
    static AppHandler* instance();
    
    DataBase* getDataBase() const;
    
    void openDataBase(std::string file);
    
    static CefRefPtr<CefV8Value> SqlResultToJSArray(SqlResult);
    static CefRefPtr<CefV8Value> SqlRowToJSObject(SqlRow);
    
protected:
    AppHandler();
    
    void emitModelChanged();
    
private:
    static AppHandler* _self;
    DataBase* _db;
};


#endif
