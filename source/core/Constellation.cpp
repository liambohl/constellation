// Constellation.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Constellation.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
ConstellationApp* application = nullptr;        // Current state of this Constellation window

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SavePrompt(HWND, UINT, WPARAM, LPARAM);
void				PromptToSaveBeforeAction(HWND hWnd, const std::function<void ()>& callback);

// Application entry point, called by OS
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Start up GDI Plus system
	Initializer gdiPlusInitializer;
	application = new ConstellationApp();

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CONSTELLATION, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONSTELLATION));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_ICON5));
    wcex.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_ICON5));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CONSTELLATION);
    wcex.lpszClassName  = szWindowClass;

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	try {
		Logger::get_instance()->log_message(message);
		switch (message)
		{
			// application menu
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
				// File menu
			case ID_FILE_NEW:
				PromptToSaveBeforeAction(hWnd, []() { application->new_drawing(); });
				break;
			case ID_FILE_OPEN:
				PromptToSaveBeforeAction(hWnd, []() { application->open(); });
				break;
			case ID_FILE_SAVE:
				application->save();
				break;
			case ID_FILE_SAVE_AS:
				application->save_as();
				break;
				// Edit menu
			case ID_EDIT_UNDO:
				application->undo();
				break;
			case ID_EDIT_REDO:
				application->redo();
				break;
				// Draw menu
			case ID_DRAW_SELECT:
				application->set_tool(ConstellationApp::SELECT);
				break;
			case ID_DRAW_PATH:
				application->set_tool(ConstellationApp::NEW_PATH);
				break;
				// Help menu
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			//case IDM_EXIT:
			//	DestroyWindow(hWnd);
			//	break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		break;

		case WM_KEYDOWN:
			switch (wParam) {
			case VK_ESCAPE: // ESC
				application->handle_escape();
				break;
			}
		break;

		// Mouse events
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			SetCapture(hWnd);	// On mouse button down, capture mouse input before handling event.
			application->handle_mouse_event(message, wParam, lParam);
			break;

		case WM_MOUSEMOVE:
			application->handle_mouse_event(message, wParam, lParam);
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			application->handle_mouse_event(message, wParam, lParam);
			ReleaseCapture();	// On mouse button up, release capture after handling event.
			break;

		// Mouse wheel events (also two-finger drag on trackpad)
		// Separated from mouse events primarily because Windows provides cursor position
		// in client coordinates for mouse events and window coordinates for mouse wheel events
		case WM_MOUSEWHEEL:		// vertical
		case WM_MOUSEHWHEEL:	// horizontal
			application->handle_mouse_wheel_event(message, wParam, lParam);
			break;

		case WM_SIZE:
			application->resize(hWnd, wParam, lParam);
			break;

		case WM_PAINT:
			application->draw(hWnd);
			break;

		case WM_CLOSE:
			PromptToSaveBeforeAction(hWnd, [hWnd]() { DestroyWindow(hWnd); });
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		if (message != WM_PAINT) {
			application->refresh_if_necessary();
		}

		return 0;
	}

	// Log all errors to errors.log
	catch (std::exception e) {
		Logger::get_instance()->log(e.what());
		return 1;
	}
}

void PromptToSaveBeforeAction(HWND hWnd, const std::function<void ()>& callback) {
	if (application->has_unsaved_changes()) {
		// Give the user a chance to save their changes and, unless they choose cancel, perform the callback.
		if (DialogBox(hInst, MAKEINTRESOURCE(IDD_SAVE_CHANGES), hWnd, SavePrompt)) {
			callback();
		}
	}
	else {
		callback();
	}
}

// Message handler for "Would you like to save your changes?" box
// Returns true if the message was handled.
// DialogBox call returns true if the operation should proceed; that is, unless "Cancel" button, "Escape" key, or "X" is pressed
INT_PTR CALLBACK SavePrompt(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return true;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:		// "Escape" key sends this command
			EndDialog(hDlg, false);
			return true;
		case IDC_SAVE:
			application->save();
			// fall through
		case IDC_DONT_SAVE:
			EndDialog(hDlg, true);
			return true;
		default:
			return false;
		}

	case WM_CLOSE:
		EndDialog(hDlg, false);
		return true;

	default:
		return false;
	}
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
