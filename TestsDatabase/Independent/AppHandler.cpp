/* (c) 2012 Álan Crístoffer */

#include "AppHandler.h"
#include "ClientHandler.h"

AppHandler* AppHandler::_self = 0;

AppHandler::AppHandler()
{
    _db = 0;
}

AppHandler* AppHandler::instance()
{
    if (!_self)
        _self = new AppHandler();
    
    return _self;
}

DataBase* AppHandler::getDataBase() const
{
    return _db;
}

void AppHandler::openDataBase(std::string file)
{
    if (_db)
        delete _db;
    
    _db = new DataBase(file);
    
    emitModelChanged();
}

void AppHandler::emitModelChanged()
{
    if ( this->clientHandler )
        this->clientHandler->modelChanged();
}
