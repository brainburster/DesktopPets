#pragma once
#include <Windows.h>

/*��ɫ*/
class Character
{
public:
	Character(HWND hwnd) :hwnd(hwnd) {}
	virtual ~Character() {};
	virtual void Logic() = 0;
	virtual void Draw()= 0;
protected:
	HWND hwnd;
};

