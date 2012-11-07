/* (c) 2012 Álan Crístoffer */

#include "ClientHandler.h"

#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "include/cef_web_urlrequest.h"
#include "include/cef_v8.h"
#include "include/cef_v8context_handler.h"

#include <iostream>
#include <sstream>

#include <AppHandler.h>
#include <DataBase.h>

extern std::string OpenFileDialog();
extern std::string SaveFileDialog();

std::string IntToString(int i)
{
    std::stringstream s;
    s << i;
    return s.str();
}

ClientHandler::ClientHandler() : m_Browser(NULL), m_BrowserHwnd(NULL)
{
    AppHandler::instance()->clientHandler = this;
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    return false;
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if (!m_Browser.get())   {
		// We need to keep the main child window, but not popup windows
        m_Browser = browser;
        m_BrowserHwnd = browser->GetWindowHandle();
    }
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    if (m_BrowserHwnd == browser->GetWindowHandle()) {
		// Free the browser pointer so that the browser can be destroyed
        m_Browser = NULL;
    }
}

bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, const CefString& url, CefRefPtr<CefClient>& client, CefBrowserSettings& settings)
{
    return false;
}

void ClientHandler::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
	// Retrieve the context's window object.
    CefRefPtr<CefV8Value> window = context->GetGlobal();
    
	// Create an instance of my CefV8Handler object.
	// In this case it's this object, and content will be executed in bool ClientHandler::Execute(...)
    CefRefPtr<CefV8Handler> handler = this;
    
	// Create a new object
    CefRefPtr<CefV8Value> cpp = CefV8Value::CreateObject(NULL);
    
	// Add the object to windows JS: window.cpp
    window->SetValue("AppCore", cpp, V8_PROPERTY_ATTRIBUTE_NONE);
    
    /*
     // Create a function.
     CefRefPtr<CefV8Value> function = CefV8Value::CreateFunction("ChangeTextInJS", handler);
     
     // Add the function to the object
     cpp->SetValue("ChangeTextInJS", function, V8_PROPERTY_ATTRIBUTE_NONE);
     */
#define ADD_FUNCTION_TO_JS(FUNCTION) cpp->SetValue(FUNCTION, CefV8Value::CreateFunction(FUNCTION, handler), V8_PROPERTY_ATTRIBUTE_NONE)
    
    ADD_FUNCTION_TO_JS("ArrayHaveElement");
    
    ADD_FUNCTION_TO_JS("checkDatabase");
    
    ADD_FUNCTION_TO_JS("categoryDelete");
    ADD_FUNCTION_TO_JS("categoryInsert");
    ADD_FUNCTION_TO_JS("categorySelect");
    ADD_FUNCTION_TO_JS("categoryUpdate");

    ADD_FUNCTION_TO_JS("listCategories");
    ADD_FUNCTION_TO_JS("listQuestions");
    
    ADD_FUNCTION_TO_JS("questionDelete");
    ADD_FUNCTION_TO_JS("questionUpdate");
    
    ADD_FUNCTION_TO_JS("OpenFileDialog");
    ADD_FUNCTION_TO_JS("SaveFileDialog");
}

void ClientHandler::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    
}

bool ClientHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    if (name == "ArrayHaveElement") {
        CefRefPtr<CefV8Value> array = arguments[0];
        std::string compare = arguments[1]->GetStringValue();
        
        retval = CefV8Value::CreateBool(false);
        
        for (int i=0; i<array->GetArrayLength(); i++) {
            std::string val = array->GetValue(i)->GetStringValue();
            if ( compare == val) {
                retval = CefV8Value::CreateBool(true);
                break;
            }
        }
        
        return true;
    }
    
    if (name == "checkDatabase") {
        retval = CefV8Value::CreateBool(AppHandler::instance()->getDataBase()->isValid());
        return true;
    }
    
    if (name == "categoryDelete") {
        int id = arguments[0]->GetIntValue();
        
        AppHandler::instance()->getDataBase()->category_delete(id);
        
        return true;
    }
    
    if (name == "categoryInsert") {
        CefString name = arguments[0]->GetStringValue();
        int parent = arguments[1]->GetIntValue();
        
        if ( name.empty() ) {
            retval = CefV8Value::CreateInt(-1);
            return true;
        }
        
        int id = AppHandler::instance()->getDataBase()->category_insert(name, parent);
        retval = CefV8Value::CreateInt(id);
        
        return true;
    }
    
    if (name == "categorySelect") {
        int id = arguments[0]->GetIntValue();
        
        SqlRow result = AppHandler::instance()->getDataBase()->category_select(id);
        retval = AppHandler::SqlRowToJSObject(result);
        
        return true;
    }
    
    if (name == "categoryUpdate") {
        int id = arguments[0]->GetIntValue();
        CefString name = arguments[1]->GetStringValue();
        int parent = arguments[2]->GetIntValue();
        
        AppHandler::instance()->getDataBase()->category_update(id, name, parent);
        
        return true;
    }
    
    if (name == "listCategories") {
        int id = arguments[0]->GetIntValue();
        
        SqlResult result = AppHandler::instance()->getDataBase()->category_select_where("parent=" + IntToString(id));
        retval = AppHandler::SqlResultToJSArray(result);
        
        return true;
    }
    
    if (name == "listQuestions") {
        int id = arguments[0]->GetIntValue();
        
        SqlResult result = AppHandler::instance()->getDataBase()->question_select_where("category=" + IntToString(id));
        retval = AppHandler::SqlResultToJSArray(result);
        
        return true;
    }
    
    if (name == "questionDelete") {
        int id = arguments[0]->GetIntValue();
        
        AppHandler::instance()->getDataBase()->question_delete(id);
        
        return true;
    }
    
    if (name == "questionUpdate") {
        int       id    = arguments[0]->GetIntValue();
        CefString title = arguments[1]->GetStringValue();
        CefString ref   = arguments[2]->GetStringValue();
        int       dif   = arguments[3]->GetIntValue();
        CefString body  = arguments[4]->GetStringValue();
        int       cat   = arguments[5]->GetIntValue();
        
        AppHandler::instance()->getDataBase()->question_update(id, title, ref, dif, body, cat);
        
        return true;
    }
    
    if (name == "OpenFileDialog") {
        AppHandler::instance()->openDataBase(OpenFileDialog());
        return true;
    }
    
    if (name == "SaveFileDialog") {
        AppHandler::instance()->openDataBase(SaveFileDialog());
        return true;
    }
    
    return false;
}

void ClientHandler::modelChanged()
{
    CefRefPtr<CefFrame> frame = this->GetBrowser()->GetMainFrame();
    
    frame->ExecuteJavaScript("App.modelChanged();", frame->GetURL(), 0);
}
