#include "MaidCat.h"
#include "Wnd.h"
#include <thread>
#include <chrono>
#include "logger.h"

MaidCat::MaidCat(Wnd * wnd):
	Character(wnd)
{
	device = GetDC(wnd->GetHWND());
	hdcImage1 = CreateCompatibleDC(device);
	hdcImage2 = CreateCompatibleDC(device);
	image1 = (HBITMAP)LoadImage(0, L"1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	image2 = (HBITMAP)LoadImage(0, L"2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(image1, sizeof(BITMAP), &bm1);
	GetObject(image2, sizeof(BITMAP), &bm2);
	SelectObject(hdcImage1, image1);
	SelectObject(hdcImage2, image2);
	MoveWindow(wnd->GetHWND(), 0, 0, bm1.bmWidth, bm1.bmHeight, false);
	Draw();
	auto thread1 = std::thread([this](){
			while (this)
			{
				if (KEY_DOWN(VK_LBUTTON)) {
					if (!pick_up) {
						pick_up = true;
						SendMessage(this->wnd->GetHWND(), WM_PAINT, 0, 0);
					}
				}
				else {
					if (pick_up) {
						pick_up = false;
						SendMessage(this->wnd->GetHWND(), WM_PAINT, 0, 0);
					}
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
		});
	thread1.detach();
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
	FillRect(device,&rect, brush);
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
	//Ã»Âß¼­
}