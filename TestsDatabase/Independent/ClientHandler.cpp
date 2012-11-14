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
    
    ADD_FUNCTION_TO_JS("answerDelete");
    ADD_FUNCTION_TO_JS("answerInsert");
    ADD_FUNCTION_TO_JS("answerSelect");
    
    ADD_FUNCTION_TO_JS("ArrayHaveElement");
    
    ADD_FUNCTION_TO_JS("checkDatabase");
    
    ADD_FUNCTION_TO_JS("categoryDelete");
    ADD_FUNCTION_TO_JS("categoryInsert");
    ADD_FUNCTION_TO_JS("categorySelect");
    ADD_FUNCTION_TO_JS("categoryUpdate");

    ADD_FUNCTION_TO_JS("listCategories");
    ADD_FUNCTION_TO_JS("listQuestions");
    ADD_FUNCTION_TO_JS("listTests");
    
    ADD_FUNCTION_TO_JS("questionDelete");
    ADD_FUNCTION_TO_JS("questionInsert");
    ADD_FUNCTION_TO_JS("questionSelect");
    ADD_FUNCTION_TO_JS("questionSelectIds");
    ADD_FUNCTION_TO_JS("questionUpdate");
    
    ADD_FUNCTION_TO_JS("testDelete");
    ADD_FUNCTION_TO_JS("testHeader");
    ADD_FUNCTION_TO_JS("testInsert");
    ADD_FUNCTION_TO_JS("testSelect");
    
    ADD_FUNCTION_TO_JS("OpenFileDialog");
    ADD_FUNCTION_TO_JS("SaveFileDialog");
}

void ClientHandler::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    
}

#define _DB_ AppHandler::instance()->getDataBase()

bool ClientHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    if (name == "answerDelete") {
        int id = arguments[0]->GetIntValue();
        
        _DB_->answer_delete(id);
        
        return true;
    }
    
    if (name == "answerInsert") {
        CefString a = arguments[0]->GetStringValue();
        int id = arguments[1]->GetIntValue();
        bool right = arguments[2]->GetBoolValue();
        
        _DB_->answer_insert(a, id, right);
        
        return true;
    }
    
    if (name == "answerSelect") {
        int id = arguments[0]->GetIntValue();
        
        SqlResult r = _DB_->answer_select(id);
        
        retval = AppHandler::SqlResultToJSArray(r);
        
        return true;
    }
    
    if (name == "ArrayHaveElement") {
        CefRefPtr<CefV8Value> array = arguments[0];
        std::string compare = arguments[1]->GetStringValue();
        
        retval = CefV8Value::CreateBool(false);
        
        for (int i=0; i < array->GetArrayLength(); i++) {
            std::string val = array->GetValue(i)->GetStringValue();
            if ( compare == val) {
                retval = CefV8Value::CreateBool(true);
                break;
            }
        }
        
        return true;
    }
    
    if (name == "checkDatabase") {
		bool ret = false;

		if ( _DB_ )
			ret = _DB_->isValid();

        retval = CefV8Value::CreateBool(ret);
        return true;
    }
    
    if (name == "categoryDelete") {
        int id = arguments[0]->GetIntValue();
        
        _DB_->category_delete(id);
        
        return true;
    }
    
    if (name == "categoryInsert") {
        CefString name = arguments[0]->GetStringValue();
        int parent = arguments[1]->GetIntValue();
        
        if ( name.empty() ) {
            retval = CefV8Value::CreateInt(-1);
            return true;
        }
        
        int id = _DB_->category_insert(name, parent);
        retval = CefV8Value::CreateInt(id);
        
        return true;
    }
    
    if (name == "categorySelect") {
        int id = arguments[0]->GetIntValue();
        
        SqlRow result = _DB_->category_select(id);
        retval = AppHandler::SqlRowToJSObject(result);
        
        return true;
    }
    
    if (name == "categoryUpdate") {
        int id = arguments[0]->GetIntValue();
        CefString name = arguments[1]->GetStringValue();
        int parent = arguments[2]->GetIntValue();
        
        _DB_->category_update(id, name, parent);
        
        return true;
    }
    
    if (name == "listCategories") {
        int id = arguments[0]->GetIntValue();
        
        SqlResult result = _DB_->category_select_where("parent=" + IntToStdString(id));
        retval = AppHandler::SqlResultToJSArray(result);
        
        return true;
    }
    
    if (name == "listQuestions") {
        int id = arguments[0]->GetIntValue();
        
        SqlResult result = _DB_->question_select_where("category=" + IntToStdString(id));
        retval = AppHandler::SqlResultToJSArray(result);
        
        return true;
    }
    
    if (name == "listTests") {
        SqlResult result = _DB_->test_select_where("");
        retval = AppHandler::SqlResultToJSArray(result);
        
        return true;
    }
    
    if (name == "questionDelete") {
        int id = arguments[0]->GetIntValue();
        
        _DB_->question_delete(id);
        
        return true;
    }
    
    if (name == "questionInsert") {
        CefString title = arguments[0]->GetStringValue();
        CefString ref   = arguments[1]->GetStringValue();
        int       dif   = arguments[2]->GetIntValue();
        CefString body  = arguments[3]->GetStringValue();
        int       cat   = arguments[4]->GetIntValue();
        
        int id = _DB_->question_insert(title, ref, dif, body, cat);
        
        retval = CefV8Value::CreateInt(id);
        
        return true;
    }
    
    if (name == "questionSelect") {
        int id = arguments[0]->GetIntValue();
        
        SqlRow r = _DB_->question_select(id);
        
        retval = AppHandler::SqlRowToJSObject(r);
        
        return true;
    }
    
    if (name == "questionSelectIds") {
        int l = arguments[0]->GetArrayLength();
        int from = arguments[1]->GetIntValue();
        int to = arguments[2]->GetIntValue();
        
        if ( from > to ) {
            int z = from;
            from = to;
            to = z;
        }
        
        std::string ids = arguments[0]->GetValue(0)->GetStringValue();
        
        for (int i=1; i < l; i++)
            ids +=  std::string(",") + std::string(arguments[0]->GetValue(i)->GetStringValue());
        
        SqlResult r = _DB_->question_select_where("category in (" + ids + ") AND difficulty BETWEEN " + IntToStdString(from) + " AND " + IntToStdString(to));
        
        CefRefPtr<CefV8Value> array = CefV8Value::CreateArray(r.size());
        
        SqlResult::iterator it;
        int i = 0;
        for ( it = r.begin() ; it < r.end(); it++ )
            array->SetValue(i++, CefV8Value::CreateString((*it)["id"]));
        
        retval = array;
        
        return true;
    }
    
    if (name == "questionUpdate") {
        int       id    = arguments[0]->GetIntValue();
        CefString title = arguments[1]->GetStringValue();
        CefString ref   = arguments[2]->GetStringValue();
        int       dif   = arguments[3]->GetIntValue();
        CefString body  = arguments[4]->GetStringValue();
        int       cat   = arguments[5]->GetIntValue();
        
        _DB_->question_update(id, title, ref, dif, body, cat);
        
        return true;
    }
    
    if (name == "testDelete") {
        int id = arguments[0]->GetIntValue();
        
        _DB_->test_delete(id);
        
        return true;
    }
    
    if (name == "testHeader") {
        SqlRow r = _DB_->test_header();
        
        retval = CefV8Value::CreateString(r["body"]);
        
        return true;
    }
    
    if (name == "testInsert") {
        std::string title = arguments[0]->GetStringValue();
        std::string body  = arguments[1]->GetStringValue();
        std::string header = arguments[2]->GetStringValue();
        
        int r = _DB_->test_insert(title, body, header);
        
        retval = CefV8Value::CreateInt(r);
        
        return true;
    }
    
    if (name == "testSelect") {
        int id = arguments[0]->GetIntValue();
        
        SqlRow r = _DB_->test_select(id);
        
        retval = AppHandler::SqlRowToJSObject(r);
        
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
