#include <assert.h>
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
#include "utilities.h"

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

    PWSTR file_name_only(const PWSTR& path) {
        PWSTR file_name = wcsrchr(path, L'\\');
        if (file_name != NULL)
            file_name++; // Move past the backslash character
        else
            file_name = path; // No backslash found, the whole path is the filename

        return file_name;
    }

    enum choose_file_mode {
        open,
        save
    };

    bool choose_cst_file(enum choose_file_mode mode, PWSTR* file_name, IShellItem* folder);

    bool open_cst_file(PWSTR* file_path, IShellItem* folder) {
        return choose_cst_file(open, file_path, folder);
    }

    bool save_as_cst_file(PWSTR* file_path, IShellItem* folder) {
        return choose_cst_file(save, file_path, folder);
    }

    // Use the Windows Common Item Dialog to obtain a file path.
    // If a file is selected, set file_name and folder accordingly.
    // Otherwise, no change.
    bool choose_cst_file(enum choose_file_mode mode, PWSTR* file_path, IShellItem* folder) {
        assert(file_path != nullptr);

        IFileDialog* pfd = NULL;
        IID dialog_id;
        if (mode == open)
            dialog_id = CLSID_FileOpenDialog;
        else
            dialog_id = CLSID_FileSaveDialog;

        // CoCreate the Dialog object.
        HRESULT hr = CoCreateInstance(dialog_id,
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
                                        // Set the file name, if one was given.
                                        if (mode == save && *file_path != nullptr) {
                                            pfd->SetFolder(folder);
                                            PWSTR file_name = file_name_only(*file_path);
                                            pfd->SetFileName(file_name);
                                        }
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
                                                PWSTR pszFilePath = NULL;
                                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                                    &pszFilePath);
                                                if (SUCCEEDED(hr))
                                                {
                                                    size_t count = wcslen(pszFilePath) + 1;
                                                    *file_path = new WCHAR[count];
                                                    wcscpy_s(*file_path, count, pszFilePath);
                                                    CoTaskMemFree(pszFilePath);
                                                    psiResult->GetParent(&folder);
                                                    return true;
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
        return false;
	}

}