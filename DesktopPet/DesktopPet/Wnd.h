#pragma once
#include <Windows.h>
#include <unordered_map>
#include <functional>

/*´°¿Ú*/
class Wnd
{
public:
	Wnd(HINSTANCE hInstance, int width, int height);
	~Wnd();

	const HWND & GetHWND() const;
	const HINSTANCE & GetHInstance() const;
	static std::unordered_map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>> * WndProcs;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void RegisterWndProc(UINT message,std::function<bool(HWND, WPARAM, LPARAM)> wndProc);
private:
	HINSTANCE m_hInstance;
	TCHAR m_szTitle[20];
	TCHAR m_szWindowClass[20];
	HWND m_hWnd;

	int m_height;
	int m_width;
};

