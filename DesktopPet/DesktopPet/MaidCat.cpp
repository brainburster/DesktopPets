#include "resource.h"
#include "MaidCat.h"
#include "Wnd.h"
#include <thread>
#include <chrono>
#include "logger.h"

#define KEY_DOWN(vk_code)(GetAsyncKeyState(vk_code) & 0x8000)

MaidCat::MaidCat(Wnd* wnd) :
	Character(wnd)
{
	device = GetDC(wnd->GetHWND());
	hdcImage1 = CreateCompatibleDC(device);
	hdcImage2 = CreateCompatibleDC(device);
	image1 = LoadBitmap(wnd->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP1));
	image2 = LoadBitmap(wnd->GetHInstance(), MAKEINTRESOURCE(IDB_BITMAP2));
	GetObject(image1, sizeof(BITMAP), &bm1);
	GetObject(image2, sizeof(BITMAP), &bm2);
	SelectObject(hdcImage1, image1);
	SelectObject(hdcImage2, image2);
	MoveWindow(wnd->GetHWND(), 0, 0, max(bm1.bmWidth, bm2.bmWidth), max(bm1.bmHeight, bm2.bmHeight), false);
	Draw();
	wnd->RegisterWndProc(WM_LBUTTONDOWN, [wnd, this](auto wparam, auto lparam) {
		if (!pick_up)
		{
			pick_up = true;
			SendMessage(wnd->GetHWND(), WM_PAINT, 0, 0);
		}
		return true;
		});
}

MaidCat::~MaidCat()
{
	DeleteDC(hdcImage1);
	DeleteDC(hdcImage2);
	DeleteObject(image1);
	DeleteObject(image2);
}

void MaidCat::Draw()
{
	//ÇåÆÁ
	RECT rect;
	GetClientRect(wnd->GetHWND(), &rect);
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	FillRect(device, &rect, brush);
	DeleteObject(brush);

	//»æÖÆÍ¼Æ¬
	if (pick_up) {
		BitBlt(device, 0, 0, bm2.bmWidth, bm2.bmHeight, hdcImage2, 0, 0, SRCCOPY);
	}
	else {
		BitBlt(device, 0, 0, bm1.bmWidth, bm1.bmHeight, hdcImage1, 0, 0, SRCCOPY);
	}
}

void MaidCat::Logic()
{
	if (!KEY_DOWN(VK_LBUTTON))
	{
		if (pick_up)
		{
			pick_up = false;
			SendMessage(wnd->GetHWND(), WM_PAINT, 0, 0);
		}
	}
}
