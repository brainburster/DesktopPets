#pragma once
#include "Character.h"
class MaidCat :
	public Character
{
public:
	MaidCat(HWND hwnd);
	~MaidCat() override;
	void Draw() override;
	void Logic() override;
private:
	HBITMAP image;
	BITMAP bm;
	HDC hdcImage;
	HDC device;
};

