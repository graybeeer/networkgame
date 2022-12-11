#pragma once

#include "Monster.h"

class Slime : public Monster {
public:
	Slime();
	Slime(const POINT& stPt);
	Slime(const POINT& stPt, int moveDir);
	virtual ~Slime();
};

