#pragma once
#include "Character.h"
#include <memory>

class MaidCatLive2D : public Character
{
public:
	MaidCatLive2D(class Wnd* wnd);
	~MaidCatLive2D() override;
	void Draw() override;
	void Logic() override;
private:
	class Impl;
	std::unique_ptr<Impl> pImpl;
};
