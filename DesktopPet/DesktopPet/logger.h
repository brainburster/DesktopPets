#pragma once

#include<string>
#include<Windows.h>

class Logger {
public:
	static void Log(std::wstring str,HWND hWnd = NULL) {
#ifdef _DEBUG
		MessageBox(hWnd, str.c_str(), L"Log", MB_OK);
#endif // _DEBUG
	}
};