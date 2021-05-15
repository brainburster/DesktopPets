#pragma once
#include "Character.h"
#include <Windows.h>

class MaidCat :
	public Character
{
public:
	MaidCat(class Wnd* wnd);
	~MaidCat() override;
	void Draw() override;
	void Logic() override;
private:
	HBITMAP image1, image2;
	BITMAP bm1, bm2;
	HDC hdcImage1, hdcImage2;
	HDC device;

	bool pick_up = false;
};
