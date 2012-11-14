/* (c) 2012 Álan Crístoffer */

#include "AppHandler.h"
#include "ClientHandler.h"

#include <iostream>
#include <sstream>
#include <string>

AppHandler* AppHandler::_self = 0;

AppHandler::AppHandler()
{
    _db = 0;
	clientHandler = 0;
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
    if ( file.empty() )
        return;
    
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

CefRefPtr<CefV8Value> AppHandler::SqlResultToJSArray(SqlResult sql)
{
    CefRefPtr<CefV8Value> jsarray = CefV8Value::CreateArray(sql.size());
    
    for ( int i = 0; i < sql.size(); i++)
        jsarray->SetValue(i, SqlRowToJSObject(sql[i]));
    
    return jsarray;
}

CefRefPtr<CefV8Value> AppHandler::SqlRowToJSObject(SqlRow sql)
{
    CefRefPtr<CefV8Value> obj = CefV8Value::CreateObject(NULL);
    
    SqlRow::iterator map;
    for ( map=sql.begin() ; map != sql.end(); map++ )
        obj->SetValue(CefString((*map).first), CefV8Value::CreateString((*map).second), V8_PROPERTY_ATTRIBUTE_NONE);
    
    return obj;
}
