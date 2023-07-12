#include <cstring>
#include <new>

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

#include "file.h"

#pragma comment(linker, "\"/manifestdependency:type='Win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Much of this file was taken from
// https://github.com/microsoft/Windows-classic-samples/blob/main/Samples/Win7Samples/winui/shell/appplatform/commonfiledialog/CommonFileDialogApp.cpp

namespace Constellation {

    const COMDLG_FILTERSPEC c_rgSaveTypes[] =
    {
        {L"Constellation Drawing (*.cst)",      L"*.cst"},
        //{L"All Documents (*.*)",                L"*.*"}
    };

    // Indices of file types
    #define INDEX_CONSTELLATION 1

    class CDialogEventHandler : public IFileDialogEvents,
        public IFileDialogControlEvents
    {
    public:
        // IUnknown methods
        IFACEMETHODIMP QueryInterface(REFIID riid, void** ppv)
        {
            static const QITAB qit[] = {
                QITABENT(CDialogEventHandler, IFileDialogEvents),
                QITABENT(CDialogEventHandler, IFileDialogControlEvents),
                { 0 },
    #pragma warning(suppress:4838)
            };
            return QISearch(this, qit, riid, ppv);
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
        IFACEMETHODIMP OnFileOk(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnFolderChange(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnFolderChanging(IFileDialog*, IShellItem*) { return S_OK; };
        IFACEMETHODIMP OnHelp(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnSelectionChange(IFileDialog*) { return S_OK; };
        IFACEMETHODIMP OnShareViolation(IFileDialog*, IShellItem*, FDE_SHAREVIOLATION_RESPONSE*) { return S_OK; };
        IFACEMETHODIMP OnTypeChange(IFileDialog* pfd);
        IFACEMETHODIMP OnOverwrite(IFileDialog*, IShellItem*, FDE_OVERWRITE_RESPONSE*) { return S_OK; };

        // IFileDialogControlEvents methods
        IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem);
        IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize*, DWORD) { return S_OK; };
        IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize*, DWORD, BOOL) { return S_OK; };
        IFACEMETHODIMP OnControlActivating(IFileDialogCustomize*, DWORD) { return S_OK; };

        CDialogEventHandler() : _cRef(1) { };
    private:
        ~CDialogEventHandler() { };
        long _cRef;
    };

    // IFileDialogEvents methods
    // This method gets called when the file-type is changed (combo-box selection changes).
    // For sample sake, let's react to this event by changing the properties show.
    HRESULT CDialogEventHandler::OnTypeChange(IFileDialog* pfd)
    {
        return 0;
    }

    // IFileDialogControlEvents
    // This method gets called when an dialog control item selection happens (radio-button selection. etc).
    // For sample sake, let's react to this event by changing the dialog title.
    HRESULT CDialogEventHandler::OnItemSelected(IFileDialogCustomize* pfdc, DWORD dwIDCtl, DWORD dwIDItem)
    {
        return 0;
    }

    // Instance creation helper
    HRESULT CDialogEventHandler_CreateInstance(REFIID riid, void** ppv)
    {
        *ppv = NULL;
        CDialogEventHandler* pDialogEventHandler = new (std::nothrow) CDialogEventHandler();
        HRESULT hr = pDialogEventHandler ? S_OK : E_OUTOFMEMORY;
        if (SUCCEEDED(hr))
        {
            hr = pDialogEventHandler->QueryInterface(riid, ppv);
            pDialogEventHandler->Release();
        }
        return hr;
    }

    // Use the Windows Open dialog to obtain a file path. Return the file path if a file was selected, else nullptr
    PWSTR open_cst_file() {
        // CoCreate the File Open Dialog object.
        IFileDialog* pfd = NULL;
        HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&pfd));
        if (SUCCEEDED(hr))
        {
            // Create an event handling object, and hook it up to the dialog.
            IFileDialogEvents* pfde = NULL;
            hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
            if (SUCCEEDED(hr))
            {
                // Hook up the event handler.
                DWORD dwCookie;
                hr = pfd->Advise(pfde, &dwCookie);
                if (SUCCEEDED(hr))
                {
                    // Set the options on the dialog.
                    DWORD dwFlags;

                    // Before setting, always get the options first in order 
                    // not to override existing options.
                    hr = pfd->GetOptions(&dwFlags);
                    if (SUCCEEDED(hr))
                    {
                        // In this case, get shell items only for file system items.
                        hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                        if (SUCCEEDED(hr))
                        {
                            // Set the file types to display only. 
                            // Notice that this is a 1-based array.
                            hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                            if (SUCCEEDED(hr))
                            {
                                // Set the selected file type index to Word Docs for this example.
                                hr = pfd->SetFileTypeIndex(INDEX_CONSTELLATION);
                                if (SUCCEEDED(hr))
                                {
                                    // Set the default extension to be ".doc" file.
                                    hr = pfd->SetDefaultExtension(L"cst");
                                    if (SUCCEEDED(hr))
                                    {
                                        // Show the dialog
                                        hr = pfd->Show(NULL);
                                        if (SUCCEEDED(hr))
                                        {
                                            // Obtain the result once the user clicks 
                                            // the 'Open' button.
                                            // The result is an IShellItem object.
                                            IShellItem* psiResult;
                                            hr = pfd->GetResult(&psiResult);
                                            if (SUCCEEDED(hr))
                                            {
                                                // We are just going to print out the 
                                                // name of the file for sample sake.
                                                PWSTR pszFilePath = NULL;
                                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                                    &pszFilePath);
                                                if (SUCCEEDED(hr))
                                                {
                                                    size_t count = wcslen(pszFilePath) + 1;
                                                    PWSTR file_path = new WCHAR[count];
                                                    wcscpy_s(file_path, count, pszFilePath);
                                                    CoTaskMemFree(pszFilePath);
                                                    return file_path;
                                                }
                                                psiResult->Release();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // Unhook the event handler.
                    pfd->Unadvise(dwCookie);
                }
                pfde->Release();
            }
            pfd->Release();
        }
        return nullptr;
	}

}