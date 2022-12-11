#pragma once

#include "AnimationManager.h"

class Effect {
public:
	Effect() = delete;
	Effect(const string& animationName);
	Effect(const string& animationName, int frameLength, float frameTime);
	Effect(const string& animationName, int frameLength,
		float frameTime, COLORREF colorKey, int alphaBlend);
	Effect(const Effect& other);
	~Effect();

	friend class EffectManager;

private:
	Animation* m_effectSrc{ };
	AnimationClip* m_effectClip{ };

	string m_clipKey{ };

	POINT m_effectPt{ };
	SIZE m_effectImgSize{ };
	SIZE m_effectBoxSize{ };

	float m_effectTime{ };
	float m_effectLimitTime{ };
	int m_effectLoop{ };
	int m_effectLoopTime{ };

	bool m_moveEffect{ false };

	COLORREF m_colorKey{ };
	int m_alphaBlend{ 0xff };
	bool m_revPrint{ false };
	bool m_rotate{ false };
	float m_angle{ };

public:
	RECT GetRC() const;
	RECT GetRenderRC() const;
	void SetClip(const string& strKey);

	bool HaveClip() const;
	void SetRev(bool rev);
	bool RevPrint() const;

	void SetRotateAngle(float angle);
	bool RotatePrint() const;

public:
	void AddNewClip(const string& strKey, const POINT& startPt, int frameLength, float frameTime);
	void SetEffectInfo(const POINT& effectPt, const SIZE& effectSize, const SIZE& effectBoxSize,
		int effectLoopTime, bool moveEffect = false);

	bool EffectEnd();
	void End();
	void Reset();

	void SetClipNull();

	void SetColorKey(COLORREF colorKey);

	void SetPt(const POINT& pt);

	void Update(float deltaTime);
	void Render(HDC destDC);
	void Render(HDC destDC, bool rev);
	void RenderRotate(HDC destDC);
};