#include "MaidCat.h"
#include <Windows.h>

MaidCat::MaidCat(HWND hwnd):
	Character(hwnd)
{
	device = GetDC(hwnd);
	image = (HBITMAP)LoadImage(0, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(image, sizeof(BITMAP), &bm);
	hdcImage = CreateCompatibleDC(device);
	SelectObject(hdcImage, image);
	MoveWindow(hwnd, 0, 0, bm.bmWidth, bm.bmHeight, false);
	Draw();
}

MaidCat::~MaidCat()
{
	DeleteDC(hdcImage);
	DeleteObject(image);
}

void MaidCat::Draw()
{
	BitBlt(device, 0, 0, bm.bmWidth, bm.bmHeight, hdcImage, 0, 0, SRCCOPY);
}

void MaidCat::Logic()
{
}
