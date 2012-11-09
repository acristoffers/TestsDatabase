#include <windows.h>      // For common windows data types and function headers
#define STRICT_TYPED_ITEMIDS
#include <shlobj.h>
#include <objbase.h>      // For COM headers
#include <shobjidl.h>     // for IFileDialogEvents and IFileDialogControlEvents
#include <shlwapi.h>
#include <knownfolders.h> // for KnownFolder APIs/datatypes/function headers
#include <propvarutil.h>  // for PROPVAR-related functions
#include <propkey.h>      // for the Property key APIs/datatypes
#include <propidl.h>      // for the Property System APIs
#include <strsafe.h>      // for StringCchPrintfW
#include <shtypes.h>      // for COMDLG_FILTERSPEC
#include <new>

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <string>

/* File Dialog Event Handler *****************************************************************************************************/

class CDialogEventHandler : public IFileDialogEvents, public IFileDialogControlEvents
{
public:
	// IUnknown methods
	IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
	{
		static const QITAB qit[] = {
			QITABENT(CDialogEventHandler, IFileDialogEvents),
			QITABENT(CDialogEventHandler, IFileDialogControlEvents),
			{ 0 },
		};
		//return QISearch(this, qit, riid, ppv);

		*ppv = NULL;
		if(riid == IID_IUnknown || riid == IID_IFileDialogEvents) {
			*ppv = static_cast<IFileDialogEvents*>(this);
			AddRef();
			return S_OK;
		}

		if(riid == IID_IUnknown || riid == IID_IFileDialogControlEvents) {
			*ppv = static_cast<IFileDialogControlEvents*>(this);
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	IFACEMETHODIMP_(ULONG) AddRef()
	{
		return InterlockedIncrement(&_cRef);
	}

	IFACEMETHODIMP_(ULONG) Release()
	{
		long cRef = InterlockedDecrement(&_cRef);
		if (!cRef)
			delete this;
		return cRef;
	}

	// IFileDialogEvents methods
	IFACEMETHODIMP OnFileOk(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnFolderChange(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnFolderChanging(IFileDialog *, IShellItem *) { return S_OK; };
	IFACEMETHODIMP OnHelp(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnSelectionChange(IFileDialog *) { return S_OK; };
	IFACEMETHODIMP OnShareViolation(IFileDialog *, IShellItem *, FDE_SHAREVIOLATION_RESPONSE *) { return S_OK; };
	IFACEMETHODIMP OnTypeChange(IFileDialog *pfd) { return S_OK; };
	IFACEMETHODIMP OnOverwrite(IFileDialog *, IShellItem *, FDE_OVERWRITE_RESPONSE *) { return S_OK; };

	// IFileDialogControlEvents methods
	IFACEMETHODIMP OnItemSelected(IFileDialogCustomize *pfdc, DWORD dwIDCtl, DWORD dwIDItem) { return S_OK; };
	IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize *, DWORD) { return S_OK; };
	IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize *, DWORD, BOOL) { return S_OK; };
	IFACEMETHODIMP OnControlActivating(IFileDialogCustomize *, DWORD) { return S_OK; };

	CDialogEventHandler() : _cRef(1) { };
private:
	~CDialogEventHandler() { };
	long _cRef;
};

std::string OpenFileDialog()
{
	IFileDialog *pfd = NULL;
	CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

	IFileDialogEvents *pfde = new CDialogEventHandler();

	DWORD dwCookie;
	pfd->Advise(pfde, &dwCookie);

	const COMDLG_FILTERSPEC c_rgSaveTypes[] = {
		{L"Tests Database File(*.qdb)",       L"*.qdb"}
	};

	DWORD dwFlags;
	pfd->GetOptions(&dwFlags);
	pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
	pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
	pfd->SetDefaultExtension(L"qdb");
	pfd->Show(NULL);

	IShellItem *psiResult;
	HRESULT hr = pfd->GetResult(&psiResult);

	if (!SUCCEEDED(hr))
		return "";

	PWSTR pszFilePath = NULL;
	psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	pfd->Unadvise(dwCookie);
	pfde->Release();
	pfd->Release();

	std::wstring wide = pszFilePath;
	std::string file(wide.begin(), wide.end());

	CoTaskMemFree(pszFilePath);

	return file;
}

std::string SaveFileDialog()
{
	IFileDialog *pfd = NULL;
	CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

	IFileDialogEvents *pfde = new CDialogEventHandler();

	DWORD dwCookie;
	pfd->Advise(pfde, &dwCookie);

	const COMDLG_FILTERSPEC c_rgSaveTypes[] = {
		{L"Tests Database File(*.qdb)",       L"*.qdb"}
	};

	DWORD dwFlags;
	pfd->GetOptions(&dwFlags);
	pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
	pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
	pfd->SetDefaultExtension(L"qdb");
	pfd->Show(NULL);

	IShellItem *psiResult;
	HRESULT hr = pfd->GetResult(&psiResult);

	if (!SUCCEEDED(hr))
		return "";

	PWSTR pszFilePath = NULL;
	psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

	pfd->Unadvise(dwCookie);
	pfde->Release();
	pfd->Release();

	std::wstring wide = pszFilePath;
	std::string file(wide.begin(), wide.end());

	CoTaskMemFree(pszFilePath);

	return file;
}

