#pragma once
#include "Monster.h"
class BadOrange : public Monster {
public:
	BadOrange();
	BadOrange(const POINT& startPt);
	BadOrange(const POINT& startPt, int moveDir);
	virtual ~BadOrange();
};

