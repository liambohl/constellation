// Constellation.cpp : Defines the entry point for the application.
//

#include <string>

#include "resource.h"

#include "ConstellationApp.h"
#include "framework.h"
#include "Initializer.h"
#include "Logger.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
ConstellationApp* application = nullptr;        // Current state of this Constellation window

// Forward declarations of functions included in this code module:
ATOM                RegisterClassConstellation(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SavePrompt(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SelectSymmetry(HWND, UINT, WPARAM, LPARAM);
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

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CONSTELLATION, szWindowClass, MAX_LOADSTRING);
	RegisterClassConstellation(hInstance);

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



// Register the window class
ATOM RegisterClassConstellation(HINSTANCE hInstance)
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

	application = new ConstellationApp(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// Window procedure
// Process messages for the main window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	try {
		Logger::get_instance()->log_message(message);
		switch (message)
		{
		// Application menu
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			// File menu
			case ID_FILE_NEW:
				PromptToSaveBeforeAction(hWnd, [hWnd]() { application->new_drawing(hWnd); });
				break;
			case ID_FILE_OPEN:
				PromptToSaveBeforeAction(hWnd, [hWnd]() { application->open(hWnd); });
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
			case ID_EDIT_CUT:
				application->cut_to_clipboard();
				break;
			case ID_EDIT_COPY:
				application->copy_to_clipboard();
				break;
			case ID_EDIT_PASTE:
				application->paste_from_clipboard(hWnd);
				break;
			case ID_EDIT_DUPLICATE:
				application->duplicate();
				break;
			case ID_EDIT_DELETE:
				application->handle_delete();
				break;
			case ID_EDIT_SELECTALL:
				application->select_all();
				break;
			case ID_EDIT_DESELECT:
				application->handle_escape();
				break;
			// Draw menu
			case ID_DRAW_SELECT:
				application->set_tool(SELECT);
				break;
			case ID_DRAW_PATH:
				application->set_tool(NEW_PATH);
				break;
			// Symmetry menu
			case ID_SYMMETRY_SYMMETRYGROUP:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_SELECT_SYMMETRY), hWnd, SelectSymmetry);
				break;
			case ID_SYMMETRY_EDITSYMMETRYPARAMETERS:
				application->set_tool(EDIT_SYMMETRY_PARAMETERS);
				break;
			// View menu
			case ID_VIEW_SYMMETRY:
			{
				// Check or uncheck menu item and show or hide symmetry guides
				HMENU hMenu = GetMenu(hWnd);
				UINT view_symmetry_state = GetMenuState(hMenu, wmId, MF_BYCOMMAND);

				if (view_symmetry_state & MF_CHECKED)
				{
					CheckMenuItem(hMenu, wmId, MF_UNCHECKED | MF_BYCOMMAND);
					application->set_view_symmetry(false);
				}
				else
				{
					CheckMenuItem(hMenu, wmId, MF_CHECKED | MF_BYCOMMAND);
					application->set_view_symmetry(true);
				}
			}
			break;
			case ID_VIEW_GHOST:
			{
				// Check or uncheck menu item and show or hide symmetry guides
				HMENU hMenu = GetMenu(hWnd);
				UINT view_ghost_state = GetMenuState(hMenu, wmId, MF_BYCOMMAND);

				if (view_ghost_state & MF_CHECKED)
				{
					CheckMenuItem(hMenu, wmId, MF_UNCHECKED | MF_BYCOMMAND);
					application->set_view_ghost(false);
				}
				else
				{
					CheckMenuItem(hMenu, wmId, MF_CHECKED | MF_BYCOMMAND);
					application->set_view_ghost(true);
				}
			}
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

		// Keystrokes
		// Note: keyboard shortcuts for application menu items are defined in the accelerator table in Constellation.rc
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

		// Set cursor
		case WM_SETCURSOR:
			application->set_cursor();
			break;

		// Resize window
		case WM_SIZE:
			application->resize(hWnd, wParam, lParam);
			break;

		// Move window
		case WM_MOVE:
			application->draw(hWnd);
			break;

		// Redraw client area
		case WM_PAINT:
			application->draw(hWnd);
			break;

		// Close application
		case WM_CLOSE:
			PromptToSaveBeforeAction(hWnd, [hWnd]() { DestroyWindow(hWnd); });
			break;

		// Destroy application
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
		*Logger::get_instance() << e.what() << std::endl;
		return 1;
	}
}

// If the current drawing has unsaved changes, prompt the user to save it.
// When done, unless they cancel, execute the callback function.
void PromptToSaveBeforeAction(HWND hWnd, const std::function<void ()>& callback) {
	if (application->has_unsaved_changes()) {
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

// Message handler for the symmetry selection dialog box
// Returns true if the message was handled.
INT_PTR CALLBACK SelectSymmetry(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		// On each button, draw an image of the corresponding symmetry group.
		std::vector<std::tuple<const wchar_t*, int, int>> symmetry_groups {
			{L"trivial",	IDC_SYMMETRY_TRIVIAL,	IDB_SYMMETRY_TRIVIAL},
			{L"p1",			IDC_SYMMETRY_P1,		IDB_SYMMETRY_P1},
			{L"p2",			IDC_SYMMETRY_P2,		IDB_SYMMETRY_P2},
			{L"pm",			IDC_SYMMETRY_PM,		IDB_SYMMETRY_PM},
			{L"pg",			IDC_SYMMETRY_PG,		IDB_SYMMETRY_PG},
			{L"cm",			IDC_SYMMETRY_CM,		IDB_SYMMETRY_CM},
			{L"pmm",		IDC_SYMMETRY_PMM,		IDB_SYMMETRY_PMM},
			{L"pmg",		IDC_SYMMETRY_PMG,		IDB_SYMMETRY_PMG},
			{L"pgg",		IDC_SYMMETRY_PGG,		IDB_SYMMETRY_PGG},
			{L"cmm",		IDC_SYMMETRY_CMM,		IDB_SYMMETRY_CMM},
			{L"p4",			IDC_SYMMETRY_P4,		IDB_SYMMETRY_P4},
			{L"p4m",		IDC_SYMMETRY_P4M,		IDB_SYMMETRY_P4M},
			{L"p4g",		IDC_SYMMETRY_P4G,		IDB_SYMMETRY_P4G},
			{L"p3",			IDC_SYMMETRY_P3,		IDB_SYMMETRY_P3},
			{L"p3m1",		IDC_SYMMETRY_P3M1,		IDB_SYMMETRY_P3M1},
			{L"p31m",		IDC_SYMMETRY_P31M,		IDB_SYMMETRY_P31M},
			{L"p6",			IDC_SYMMETRY_P6,		IDB_SYMMETRY_P6},
			{L"p6m",		IDC_SYMMETRY_P6M,		IDB_SYMMETRY_P6M}
		};
		HBITMAP bitmap;
		for (auto [text, control_id, bitmap_id] : symmetry_groups) {
			bitmap = LoadBitmap(hInst, MAKEINTRESOURCE(bitmap_id));
			SendDlgItemMessage(hDlg, control_id, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)bitmap);
		}
		RECT rect;
		GetClientRect(hDlg, &rect);
		InvalidateRect(hDlg, &rect, false);
		return true;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SYMMETRY_TRIVIAL:
			application->set_symmetry_group(TRIVIAL);
			break;
		case IDC_SYMMETRY_P1:
			application->set_symmetry_group(P1);
			break;
		case IDC_SYMMETRY_P2:
			application->set_symmetry_group(P2);
			break;
		case IDC_SYMMETRY_PM:
			application->set_symmetry_group(PM);
			break;
		case IDC_SYMMETRY_PG:
			application->set_symmetry_group(PG);
			break;
		case IDC_SYMMETRY_CM:
			application->set_symmetry_group(CM);
			break;
		case IDC_SYMMETRY_PMM:
			application->set_symmetry_group(PMM);
			break;
		case IDC_SYMMETRY_PMG:
			application->set_symmetry_group(PMG);
			break;
		case IDC_SYMMETRY_PGG:
			application->set_symmetry_group(PGG);
			break;
		case IDC_SYMMETRY_CMM:
			application->set_symmetry_group(CMM);
			break;
		case IDC_SYMMETRY_P4:
			application->set_symmetry_group(P4);
			break;
		case IDC_SYMMETRY_P4M:
			application->set_symmetry_group(P4M);
			break;
		case IDC_SYMMETRY_P4G:
			application->set_symmetry_group(P4G);
			break;
		case IDC_SYMMETRY_P3:
			application->set_symmetry_group(P3);
			break;
		case IDC_SYMMETRY_P3M1:
			application->set_symmetry_group(P3M1);
			break;
		case IDC_SYMMETRY_P31M:
			application->set_symmetry_group(P31M);
			break;
		case IDC_SYMMETRY_P6:
			application->set_symmetry_group(P6);
			break;
		case IDC_SYMMETRY_P6M:
			application->set_symmetry_group(P6M);
			break;
		}
		EndDialog(hDlg, false);
		return true;
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
