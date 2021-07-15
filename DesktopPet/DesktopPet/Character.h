#pragma once

class Character
{
public:
	Character(class Wnd* wnd) :wnd(wnd) {}
	virtual ~Character() {};
	virtual void Logic() = 0;
	virtual void Draw() = 0;
protected:
	Wnd* wnd;
};
