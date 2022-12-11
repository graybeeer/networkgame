#pragma once

#include "framework.h"
#include "AnimationClip.h"

class Animation {
public:
	Animation();
	~Animation();
	Animation(const Animation& other);

private:
	CImage* m_srcImg{ };
	POINT m_maxFrameLength{ };
	wstring m_srcFileName{ };
	map<string, AnimationClip*> m_clipMap;

public:
	POINT GetMaxFrameLength() const;
	wstring GetSrcFileName() const;

public:
	bool LoadSrcImg(CImage* srcImg, const wstring& fileName, POINT maxFrameLength);

	bool AddNewClip(const string& clipName, POINT startFrame,
		int frameLength, float frameTime);

	bool AddNewClip(const string& clipName, POINT startFrame,
		int frameLength, SIZE frameSize, float frameTime);

	bool CopyClip(const Animation* protoAnimation);

	AnimationClip* GetClip(const string& clipName);

	HDC GetDC() const;
	void ReleaseDC() const;

	void Render(HDC destDC, const RECT& destRC, const RECT& srcRC, COLORREF colorKey = 0);
	void Render(HDC destDC, const RECT& destRC, const RECT& srcRC, bool rev, COLORREF colorKey = 0);
	/*void Render(HDC destDC, const SIZE& destSZ,
		const RECT& destRC, const RECT& srcRC, bool rev, COLORREF colorKey = 0);
	void RenderRotate(HDC destDC, const RECT& destRC, const RECT& srcRC,
		const SIZE& srcSz, float angle, COLORREF colorKey = 0U);*/

	void Render(HDC destDC, const SIZE& destSZ, const RECT& destRC,
		const RECT& srcRC, bool rev, COLORREF colorKey = 0, int alphaBlend = 255);

	void RenderRotate(HDC destDC, const RECT& destRC, const RECT& srcRC,
		const SIZE& srcSz, float angle, COLORREF colorKey = 0, int alphaBlend = 255);
};

