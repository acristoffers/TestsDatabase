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

ClientHandler::ClientHandler()
: m_Browser(NULL),
m_BrowserHwnd(NULL)
{
    
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

void ClientHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 CefRefPtr<CefV8Context> context)
{
    // Retrieve the context's window object.
    CefRefPtr<CefV8Value> window = context->GetGlobal();
    
    // Create an instance of my CefV8Handler object.
    // In this case it's this object, and content will be executed in bool ClientHandler::Execute(...)
    CefRefPtr<CefV8Handler> handler = this;
    
    // Create a function.
    CefRefPtr<CefV8Value> function = CefV8Value::CreateFunction("ChangeTextInJS", handler);
    
    // Create a new object
    CefRefPtr<CefV8Value> cpp = CefV8Value::CreateObject(NULL);
    
    // Add the object to windows JS: window.cpp
    window->SetValue("cpp", cpp, V8_PROPERTY_ATTRIBUTE_NONE);
    
    // Add the function to the object
    cpp->SetValue("ChangeTextInJS", function, V8_PROPERTY_ATTRIBUTE_NONE);
}

void ClientHandler::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
    
}

bool ClientHandler::Execute(const CefString& name, CefRefPtr<CefV8Value> object, const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval, CefString& exception)
{
    if (name == "ChangeTextInJS") {
        if (arguments.size() == 1 && arguments[0]->IsString()) {
            CefString text = arguments[0]->GetStringValue();
            
            CefRefPtr<CefFrame> frame = this->GetBrowser()->GetMainFrame();
            
            std::string jscall = "ChangeText('";
            jscall += text;
            jscall += "');";
            
            frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);
            
            return true;
        }
    }
    
    return false;
}