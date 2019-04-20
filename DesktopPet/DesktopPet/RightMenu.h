#pragma once
#include "Wnd.h"

class RightMenu
{
public:
	RightMenu(Wnd * wnd);
	~RightMenu();
private:
	HMENU hMenu;
};

