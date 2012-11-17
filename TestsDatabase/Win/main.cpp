﻿#include <string>
#include <windows.h>
#include <fstream>

#include "include/cef_app.h"
#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/cef_command_line.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"
#include "include/cef_web_plugin.h"
#include "include/cef_web_urlrequest.h"

#include "ClientHandler.h"
#include "AppHandler.h"
#include "resources.h"

ClientHandler* g_handler = 0;

std::string GetApplicationDir()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR wpath[MAX_PATH];
	GetModuleFileNameW(hModule, wpath, MAX_PATH);

	std::wstring wide( wpath );
	std::string path( wide.begin(), wide.end() );

	return path.substr(0, path.find_last_of("\\/"));
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_SIZE:
		if (g_handler) {
			// Resize the browser window and address bar to match the new frame
			// window size
			RECT rect;
			GetClientRect(hwnd, &rect);

			HDWP hdwp = BeginDeferWindowPos(1);
			hdwp = DeferWindowPos(hdwp, g_handler->GetBrowserHwnd(), NULL,
				rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				SWP_NOZORDER);
			EndDeferWindowPos(hdwp);
		}
		break;

	case WM_ERASEBKGND:
		if (g_handler) {
			// Dont erase the background if the browser window has been loaded
			// (this avoids flashing)
			return 0;
		}
		break;

	case WM_CLOSE:
		if (g_handler) {
			CefRefPtr<CefBrowser> browser = g_handler->GetBrowser();
			if (browser.get()) {
				// Let the browser window know we are about to destroy it.
				browser->ParentWindowWillClose();
			}
		}
		break;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND RegisterWindow(HINSTANCE hInstance, int nCmdShow)
{
	const wchar_t CLASS_NAME[]  = L"TestsDatabaseWindow";

	WNDCLASS wc = { };

	wc.lpfnWndProc   = WindowProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYICON));
	
	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Tests Database",              // Window title
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
		);

	if (hwnd == NULL)
		return 0;

	ShowWindow(hwnd, nCmdShow);

	return hwnd;
}

extern std::string utf8_encode(const std::wstring &wstr);
extern std::wstring utf8_decode(const std::string &str);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	std::string path = GetApplicationDir();
    AppHandler::htmlFolderPath = path + "/html";
	path = "file://" + path + "/html/index.html";
    
	if ( __argc > 1 ) {
		char* file_path = __argv[1];
		int size_needed = MultiByteToWideChar(CP_ACP, 0, &file_path[0], strlen(file_path), NULL, 0);
		std::wstring wstrTo( size_needed, 0 );
		MultiByteToWideChar(CP_ACP, 0, &file_path[0], strlen(file_path), &wstrTo[0], size_needed);
		
		std::string utf8 = utf8_encode(wstrTo);

		AppHandler::instance()->openDataBase( utf8 );
	}

	// Register the window class.
	HWND hwnd = RegisterWindow(hInstance, nCmdShow);
	if( hwnd == 0 )
		return 0;

	RECT rect;
	GetClientRect(hwnd, &rect);

	CefSettings appSettings;
	CefBrowserSettings browserSettings;

	appSettings.log_severity = LOGSEVERITY_DISABLE;
	CefString(&appSettings.log_file) = AppHandler::htmlFolderPath + "/../debug.log";
	
	CefRefPtr<CefApp> cefApplication;
	CefRefPtr<CefClient> client(new ClientHandler());
	g_handler = (ClientHandler*) client.get();

	CefWindowInfo info;
	info.SetAsChild(hwnd, rect);

	CefInitialize(appSettings, cefApplication);

	CefBrowser::CreateBrowser(info, client, path, browserSettings);

	CefRunMessageLoop();

	CefShutdown();

	return 0;
}
