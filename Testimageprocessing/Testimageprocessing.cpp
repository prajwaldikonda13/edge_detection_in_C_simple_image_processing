#include"stdafx.h"
#include"includes.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR szCmdLine, int nCmdShow)
{

	TCHAR appName[] = TEXT("WindowsApp");
	WNDCLASSEX wndClassEx;
	HWND hWnd;
	MSG msg;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClassEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClassEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wndClassEx.hInstance = hInstance;
	wndClassEx.lpfnWndProc = WndProc;
	wndClassEx.lpszClassName = appName;
	wndClassEx.lpszMenuName = NULL;
	wndClassEx.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClassEx(&wndClassEx);
	hWnd = CreateWindow(appName, TEXT("WindowsName"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1360, 760, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, hWnd, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	DestroyWindow(hWnd);
	return msg.wParam;
}
HDC hdc;
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	switch (msg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		processImage(hdc);
		GetClientRect(hWnd, &rect);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}