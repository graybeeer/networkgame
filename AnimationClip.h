#pragma once

#include "framework.h"


class AnimationClip {
public:
	AnimationClip();
	AnimationClip(const AnimationClip& other);
	~AnimationClip();

private:
	SIZE m_frameSize{ };

	int m_frameLengthOrigin{ };
	float m_frameTimeOrigin{ };
	POINT m_startFrameOrigin{ };

	POINT m_startFrame{ };
	int m_frameLength{ };
	POINT m_curFrame{ };
	POINT m_maxFrameLength{ };

	float m_animationTime{ };
	float m_animationLoopTime{ };
	float m_frameTime{ };

	bool m_frameStop{ false };

	// int m_loopSize{ };

public:
	bool CreateNewClip(CImage* srcImg,	POINT startFrame, int frameLength,
		POINT maxFrameLength, float frameTime);

	bool CreateNewClip(CImage* srcImg, POINT startFrame, int frameLength,
		POINT maxFrameLength, SIZE frameSize, float frameTime);
	
	POINT GetFramePos() const;
	SIZE GetFrameSize() const;
	RECT GetImgRC() const;

	void SetFrameTime(float frameTime);
	void SetFrameLength(int frameLength);
	void SetFrameStartPos(const POINT& stPt);

	void StopFrameUpdate();
	void ActiveFrameUpdate();

	float GetLoopTime() const;

	void Update(float deltaTime);
	void Reset();
};
