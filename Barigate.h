#pragma once

#include "framework.h"
#include "Global.h"
#include "TextureManager.h"
#include "AnimationManager.h"

class Barigate {
public:
	Barigate();
	Barigate(const POINT& pt, const SIZE& size);
	Barigate(const string& imgTag, const POINT& pt);
	Barigate(const string& imgTag, const POINT& pt, const SIZE& size);

	~Barigate();

private:
	string m_imgTag{ };
	Animation* m_animation{ };
	AnimationClip* m_curClip{ };
	CImage* m_img{ };
	POINT m_pt{ };
	SIZE m_imgSize{ };

	SIZE m_hitBoxSize{ };

public:
	POINT GetPt() const;
	RECT GetRC() const;
	SIZE GetSize() const;

public:
	void Update(float deltaTime);
	void Render(HDC destDC);
	void Collision();

	bool Save(FILE* fp);

	bool Load(FILE* fp);

};