#pragma once
#include "TextureManager.h"

#include "Animation.h"
#include "Player.h"
#include "Global.h"


class Portal {
public:
	Portal();
	Portal(const POINT& pt);
	~Portal();

private:
	string m_imgTag{ };
	Animation* m_animation{ };
	AnimationClip* m_curClip{ };
	CImage* m_img{ };
	POINT m_pt{ };
	POINT m_nextStageCameraPos{ };	// 다음 스테이지 카메라 위치 (쓰나?)
	SIZE m_imgSize{ 64, 64 };


	SIZE m_hitBoxSize{ 64, 64 };

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