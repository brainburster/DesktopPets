#pragma once

#include<string>
#include<Windows.h>

class Logger {
public:
	static void Log(std::string str, HWND hWnd = NULL) {
#ifdef _DEBUG
		MessageBox(hWnd, str.c_str(), TEXT("Log"), MB_OK);
#endif // _DEBUG
	}
};
