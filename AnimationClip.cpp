#include "AnimationClip.h"

AnimationClip::AnimationClip() { }

AnimationClip::AnimationClip(const AnimationClip& other) {
	m_animationTime = 0.f;
	m_frameTime = other.m_frameTime;
	m_maxFrameLength = other.m_maxFrameLength;

	m_startFrame = other.m_startFrame;
	m_curFrame = m_startFrame;
	m_frameLength = other.m_frameLength;

	m_frameTimeOrigin = m_frameTime;
	m_frameLengthOrigin = m_frameLength;
	m_startFrameOrigin = m_startFrameOrigin;

	m_animationLoopTime = m_frameLength * m_frameTime;

	m_frameSize = other.m_frameSize;
}

AnimationClip::~AnimationClip() { }

bool AnimationClip::CreateNewClip(CImage* srcImg, POINT startFrame, int frameLength,
	POINT maxFrameLength, float frameTime) {
	m_animationTime = 0.f;
	m_frameTime = frameTime;

	m_maxFrameLength = maxFrameLength;

	m_startFrame = startFrame;
	m_curFrame = startFrame;
	m_frameLength = frameLength;

	m_frameTimeOrigin = m_frameTime;
	m_frameLengthOrigin = m_frameLength;
	m_startFrameOrigin = m_startFrameOrigin;

	m_animationLoopTime = m_frameLength * m_frameTime;

	SIZE imgSize = SIZE{ srcImg->GetWidth(), srcImg->GetHeight() };
	m_frameSize = { imgSize.cx / m_maxFrameLength.x, imgSize.cy / m_maxFrameLength.y };

	return true;
}

bool AnimationClip::CreateNewClip(CImage* srcImg, POINT startFrame, int frameLength,
	POINT maxFrameLength, SIZE frameSize, float frameTime) {
	m_animationTime = 0.f;
	m_frameTime = frameTime;

	m_startFrame = startFrame;
	m_curFrame = startFrame;
	m_frameLength = frameLength;

	m_maxFrameLength = maxFrameLength;

	m_frameTimeOrigin = m_frameTime;
	m_frameLengthOrigin = m_frameLength;
	m_startFrameOrigin = m_startFrameOrigin;

	m_animationLoopTime = m_frameLength * m_frameTime;

	SIZE imgSize = SIZE{ srcImg->GetWidth(), srcImg->GetHeight() };
	m_frameSize = { imgSize.cx / m_maxFrameLength.x, imgSize.cy / m_maxFrameLength.y };

	return true;
}

POINT AnimationClip::GetFramePos() const {
	return m_curFrame;
}

SIZE AnimationClip::GetFrameSize() const {
	return m_frameSize;
}

RECT AnimationClip::GetImgRC() const {
	return RECT{ m_curFrame.x * m_frameSize.cx, m_curFrame.y * m_frameSize.cy,
		(m_curFrame.x + 1) * m_frameSize.cx, (m_curFrame.y + 1) * m_frameSize.cy };
}

void AnimationClip::SetFrameTime(float frameTime) {
	m_frameTime = frameTime;
}

void AnimationClip::SetFrameLength(int frameLength) {
	m_frameLength = frameLength;
}

void AnimationClip::SetFrameStartPos(const POINT& stPt) {
	m_startFrame = stPt;
}

void AnimationClip::StopFrameUpdate() {
	m_frameStop = true;
}

void AnimationClip::ActiveFrameUpdate() {
	m_frameStop = false;
}

float AnimationClip::GetLoopTime() const {
	return m_animationLoopTime;
}

void AnimationClip::Update(float deltaTime) {
	if (m_frameStop) {
		return;
	}

	m_animationTime += deltaTime;
	if (m_animationTime >= m_frameTime) {
		m_curFrame.x++;


		if (m_curFrame.x == m_maxFrameLength.x) {
			m_curFrame.x = 0;
			m_curFrame.y++;
		}
		
		int frameAdvance = (m_curFrame.y - m_startFrame.y) * m_maxFrameLength.x
			+ m_curFrame.x - m_startFrame.x;

		if (frameAdvance == m_frameLength) {
			m_curFrame = m_startFrame;
		}

		m_animationTime = 0.f;
	}
}

void AnimationClip::Reset() {
	m_animationTime = 0.f;
	m_curFrame = m_startFrame;
}
