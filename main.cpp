#define UNICODE
#pragma comment(linker,"/opt:nowin98")
#include<windows.h>

TCHAR szClassName[]=TEXT("Window");

LRESULT CALLBACK WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hEdit;
	switch(msg)
	{
	case WM_CREATE:
		hEdit=CreateWindow(TEXT("EDIT"),0,WS_CHILD|WS_VISIBLE|ES_READONLY|ES_AUTOHSCROLL,10,10,256,32,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		CreateWindow(TEXT("STATIC"),TEXT("このウィンドウがアクティブな状態で、何かキーを押すとクリップボードにRGB情報をコピーします。"),WS_CHILD|WS_VISIBLE,10,50,256,64,hWnd,0,((LPCREATESTRUCT)lParam)->hInstance,0);
		SetTimer(hWnd,0x1234,10,0);
		break;
	case WM_TIMER:
		{
			POINT point;
			GetCursorPos(&point);
			const HWND hDesktop=GetDesktopWindow();
			const HDC hdc=GetDC(hDesktop);
			const COLORREF color=GetPixel(hdc,point.x,point.y);
			TCHAR szText[1024];
			wsprintf(szText,TEXT("RGB(%d,%d,%d);"),GetRValue(color),GetGValue(color),GetBValue(color));
			SetWindowText(hEdit,szText);
			ReleaseDC(hDesktop,hdc);
		}
		break;
	case WM_KEYDOWN:
		SendMessage(hEdit,EM_SETSEL,0,-1);
		SendMessage(hEdit,WM_COPY,0,0);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 0x1234);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,msg,wParam,lParam);
	}
	return 0;
}

EXTERN_C void __cdecl WinMainCRTStartup()
{
	MSG msg;
	HINSTANCE hInstance=GetModuleHandle(0);
	WNDCLASS wndclass={
		CS_HREDRAW|CS_VREDRAW,
		WndProc,
		0,
		0,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW+1),
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	HWND hWnd=CreateWindow(
			szClassName,
			TEXT("Window"),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			0,
			CW_USEDEFAULT,
			0,
			0,
			0,
			hInstance,
			0
		);
	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while(GetMessage(&msg,0,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(msg.wParam);
}

#if _DEBUG
void main(){}
#endif
