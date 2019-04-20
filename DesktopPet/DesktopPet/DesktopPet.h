#pragma once

#include "Wnd.h"
#include "Character.h"
#include "RightMenu.h"

class DesktopPet
{
public:
	DesktopPet(HINSTANCE hInstance,const TCHAR * pictureName, int width, int height);
	~DesktopPet();
	int Run();
private:
	bool Init();
	void Loop();
	
	Wnd * wnd;
	Character * character;
	RightMenu * rightMenu;

	bool close;
	MSG m_msg;
};

