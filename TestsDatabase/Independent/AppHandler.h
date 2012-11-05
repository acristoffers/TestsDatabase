/* (c) 2012 Álan Crístoffer */

#ifndef APPHANDLER_H
#define APPHANDLER_H

#include <string>

#include "DataBase.h"

class ClientHandler;

class AppHandler {
public:
    ClientHandler* clientHandler;
    static AppHandler* instance();
    
    DataBase* getDataBase() const;
    
    void openDataBase(std::string file);
    
protected:
    AppHandler();
    
    void emitModelChanged();
    
private:
    static AppHandler* _self;
    DataBase* _db;
};


#endif
