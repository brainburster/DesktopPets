#pragma once
#include <Windows.h>
#include <functional>
#include <map>

/*´°¿Ú*/
class Wnd
{
public:
	Wnd(HINSTANCE hInstance, int width, int height);
	~Wnd();

	const HWND & GetHWND() const;
	const HINSTANCE & GetHInstance() const;
	static std::map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>> WndProcs;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void RegisterWndProc(UINT message,const std::function<bool(HWND, WPARAM, LPARAM)> & wndProc);
	void peekMessage();
private:
	HINSTANCE m_hInstance;
	TCHAR m_szTitle[20];
	TCHAR m_szWindowClass[20];
	HWND m_hWnd;

	int m_height;
	int m_width;
};

