#pragma once
#include <Windows.h>

/*��ɫ*/
class Character
{
public:
	Character(HWND hwnd,const TCHAR * pictureName);
	~Character();
	void Logic();
	void Draw();
private:
	HBITMAP image;
	BITMAP bm;
	HDC hdcImage;
	HDC device;
};

