#pragma once

#include "Wnd.h"
#include "Character.h"
#include "RightMenu.h"
#include "logger.h"
#include <time.h>

template<typename T = Character>
class DesktopPet
{
public:
	DesktopPet(HINSTANCE hInstance, UINT x, UINT y);
	~DesktopPet();
	int Run();
private:
	bool Init();
	void Loop();

	Wnd wnd;
	T character;
	RightMenu* rightMenu;

	bool bCloseApp;
};

template<typename T>
DesktopPet<T>::DesktopPet(HINSTANCE hInstance, UINT x, UINT y) :
	bCloseApp(false),
	wnd(hInstance, x, y),
	character(&wnd)
{
	rightMenu = new RightMenu(&wnd);
}

template<typename T>
DesktopPet<T>::~DesktopPet()
{
	delete rightMenu;
}

template<typename T>
int DesktopPet<T>::Run()
{
	if (!Init()) {
		return 0;
	}
	Loop();
	return 0;
}

template<typename T>
bool DesktopPet<T>::Init()
{
	//只允许存在一个实例
	auto flag = CreateMutex(NULL, TRUE, TEXT("DesktopPet"));
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
	wnd.RegisterWndProc(WM_DESTROY, [this](auto, auto) {
		bCloseApp = true;
		return true;
		});
	//渲染
	wnd.RegisterWndProc(WM_PAINT, [this](auto, auto) {
		character.Draw();
		return true;
		});
	//按住可拖拽
	wnd.RegisterWndProc(WM_LBUTTONDOWN, [this](auto, auto) {
		SendMessage(wnd.GetHWND(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return true;
		});
	return true;
}

template<typename T>
void DesktopPet<T>::Loop()
{
	clock_t previous = clock();
	clock_t lag = 0;
	clock_t delay = 1000 / 30;
	while (!bCloseApp)
	{
		wnd.peekMessage();
		clock_t current = clock();
		clock_t elapsed = current - previous;
		previous = current;
		lag += elapsed;
		while (lag >= delay) {
			character.Logic();
			lag -= delay;
		}
		character.Draw();
		Sleep(1);//减少cpu占用
	}
}
