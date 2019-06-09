#pragma once

#include "Wnd.h"
#include "Character.h"
#include "RightMenu.h"
#include "logger.h"
#include "Resource.h"

template<typename T = Character>
class DesktopPet
{
public:
	DesktopPet(HINSTANCE hInstance);
	~DesktopPet();
	int Run();
private:
	bool Init();
	void Loop();
	
	Wnd * wnd;
	Character* character;
	RightMenu * rightMenu;

	bool close;
	MSG m_msg;
};

template<typename T>
DesktopPet<T>::DesktopPet(HINSTANCE hInstance) :
	close(false),
	m_msg()
{
	wnd = new Wnd(hInstance, 800, 600);
	character = new T(wnd->GetHWND());
	rightMenu = new RightMenu(wnd);
}

template<typename T>
DesktopPet<T>::~DesktopPet()
{
	delete wnd;
	delete character;
	delete rightMenu;
}

template<typename T>
int DesktopPet<T>::Run()
{
	if (!Init()) {
		return 0;
	}
	Loop();
	return m_msg.wParam;
}

template<typename T>
bool DesktopPet<T>::Init()
{
	//只允许存在一个实例
	auto flag = CreateMutex(NULL, TRUE, L"DesktopPet");
	auto err = GetLastError();
	if (err == ERROR_ALREADY_EXISTS)
	{
		Logger::Log(TEXT("已存在一个实例"));
		return false;
	}
	if (flag)
	{
		ReleaseMutex(flag);
	}

	//关闭
	wnd->RegisterWndProc(WM_DESTROY, [this](auto hwnd, auto, auto) {
		close = true;
		return true;
		});
	////渲染
	wnd->RegisterWndProc(WM_PAINT, [this](auto hwnd, auto, auto) {
		character->Draw();
		return true;
		});
	//按住可拖拽
	wnd->RegisterWndProc(WM_LBUTTONDOWN, [this](auto hwnd, auto, auto) {
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return true;
		});
	wnd->RegisterWndProc(WM_LBUTTONDBLCLK, [this](auto hwnd, auto, auto) {
		Logger::Log(L"你双击了", hwnd);
		return true;
		});
	return true;
}

template<typename T>
void DesktopPet<T>::Loop()
{
	while (!close)
	{
		if (PeekMessage(&m_msg, wnd->GetHWND(), 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}
}
