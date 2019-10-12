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
	DesktopPet(HINSTANCE hInstance);
	~DesktopPet();
	int Run();
private:
	bool Init();
	void Loop();
	
	Wnd wnd;
	T character;
	RightMenu * rightMenu;

	bool bCloseApp;
};

template<typename T>
DesktopPet<T>::DesktopPet(HINSTANCE hInstance) :
	bCloseApp(false),
	wnd(hInstance, 800, 600),
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
	//ֻ�������һ��ʵ��
	auto flag = CreateMutex(NULL, TRUE, L"DesktopPet");
	auto err = GetLastError();
	if (err == ERROR_ALREADY_EXISTS)
	{
		Logger::Log(TEXT("�Ѵ���һ��ʵ��"));
		return false;
	}
	if (flag)
	{
		ReleaseMutex(flag);
	}

	//�ر�
	wnd.RegisterWndProc(WM_DESTROY, [this](auto hwnd, auto, auto) {
		bCloseApp = true;
		return true;
		});
	//////��Ⱦ
	wnd.RegisterWndProc(WM_PAINT, [this](auto hwnd, auto, auto) {
		character.Draw();
		return true;
		});
	////��ס����ק
	wnd.RegisterWndProc(WM_LBUTTONDOWN, [](auto hwnd, auto, auto) {
		SendMessage(hwnd, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
		return true;
	});
	return true;
}

template<typename T>
void DesktopPet<T>::Loop()
{
	clock_t previous = clock();
	clock_t lag = 0;
	clock_t delay = 33;
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
		//character.Draw();
		Sleep(1);//����cpuռ��
	}
}
