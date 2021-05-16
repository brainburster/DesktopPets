#pragma once
#include <Windows.h>
#include <functional>
#include <map>
#include <tuple>

using MSG_ID = std::tuple<HWND, UINT>;
using MSG_Handler = std::function<bool(WPARAM, LPARAM)>;
using MSG_Table = std::map<MSG_ID, MSG_Handler>;

/*´°¿Ú*/
class Wnd
{
public:
	Wnd(HINSTANCE hInstance, int width, int height);
	~Wnd();

	const HWND& GetHWND() const;
	const HINSTANCE& GetHInstance() const;
	static MSG_Table msg_table;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void RegisterWndProc(UINT message, const MSG_Handler& wndProc);
	void peekMessage();
private:
	HINSTANCE m_hInstance;
	char m_szTitle[20];
	char m_szWindowClass[20];
	HWND m_hWnd;

	int m_height;
	int m_width;
};
