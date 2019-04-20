#include "Character.h"



Character::Character(HWND hwnd,const TCHAR * pictureName)
{
	device = GetDC(hwnd);
	image = (HBITMAP)LoadImage(0, pictureName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(image, sizeof(BITMAP), &bm);
	hdcImage = CreateCompatibleDC(device);
	SelectObject(hdcImage, image);
	Draw();
}

Character::~Character()
{
	DeleteDC(hdcImage);
	DeleteObject(image);
}

void Character::Draw()
{
	BitBlt(device, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
}

void Character::Logic()
{
}