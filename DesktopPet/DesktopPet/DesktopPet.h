#pragma once
#include <Windows.h>
#include <unordered_map>
#include <functional>

class DesktopPet
{
public:
	DesktopPet(HINSTANCE hInstance);
	~DesktopPet();
	int Run();
	static std::unordered_map<UINT, std::function<bool(HWND, WPARAM, LPARAM)>> * WndProcs;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool Start();
	void End();
	void Loop();
	void Update();

	HINSTANCE m_hInstance;
	TCHAR m_szTitle[20];
	TCHAR m_szWindowClass[20];
	HWND m_hWnd;
	MSG m_msg;
	int m_height = 600;
	int m_width = 400;
	bool close;

	HBITMAP image;
	BITMAP bm;
	HDC hdcImage;
};

