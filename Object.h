#pragma once

#include "framework.h"
#include "AnimationManager.h"
#include "Hpbar.h"
#include "Effect.h"

class Object {
public:
	Object();
	Object(const POINT& pt, const SIZE& sz, const SIZE& hbSz, const string& tag);
	virtual ~Object();

protected:
	SIZE m_hitBoxSize{ };
	POINT m_pt{ };
	SIZE m_size{ };

	string m_tag{ };

public:
	string GetTag() const;
	POINT GetPt() const;
	SIZE GetSize() const;
	RECT GetHitBox() const;
	RECT GetRC() const;

	void SetPt(const POINT& newPt);
	
public:
	void SetDefaultInfo(const POINT& pt, const SIZE& sz, const SIZE& hbSz);

	virtual void Update(float deltaTime) = 0;
	virtual void Render(HDC destDC) = 0;
	virtual void Collision(const string& strTag) = 0;
	virtual void Input(float deltaTime);

};