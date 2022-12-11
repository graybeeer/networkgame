#include "Effect.h"

Effect::Effect(const string& animationName) {
	m_effectSrc = AnimationManager::GetInst()->GetAnimation(animationName);
}

Effect::Effect(const string& animationName, int frameLength, float frameTime) {
	m_effectSrc = AnimationManager::GetInst()->GetAnimation(animationName);
	m_effectSrc->AddNewClip(animationName, POINT{ 0, 0 }, frameLength, frameTime);
	m_clipKey = animationName;
	m_effectClip = m_effectSrc->GetClip(animationName);
	m_effectLimitTime = m_effectClip->GetLoopTime();
}

Effect::Effect(const string& animationName, int frameLength,
	float frameTime, COLORREF colorKey, int alphaBlend) {
	m_effectSrc = AnimationManager::GetInst()->GetAnimation(animationName);
	m_effectSrc->AddNewClip(animationName, POINT{ 0, 0 }, frameLength, frameTime);
	m_clipKey = animationName;
	m_effectClip = m_effectSrc->GetClip(animationName);
	m_effectLimitTime = m_effectClip->GetLoopTime();
	m_alphaBlend = alphaBlend;
	m_colorKey = colorKey;
}

Effect::Effect(const Effect& other) {
	m_effectSrc = AnimationManager::GetInst()->GetAnimation(other.m_effectSrc);
	m_effectSrc->CopyClip(other.m_effectSrc);
	m_clipKey = other.m_clipKey;
	m_effectClip = m_effectSrc->GetClip(m_clipKey);

	m_effectPt = other.m_effectPt;
	m_effectImgSize = other.m_effectImgSize;
	m_effectBoxSize = other.m_effectBoxSize;

	m_effectTime = other.m_effectTime;
	m_effectLimitTime = other.m_effectLimitTime;
	m_effectLoop = other.m_effectLoop;
	m_effectLoopTime = other.m_effectLoopTime;

	m_moveEffect = other.m_moveEffect;
	m_alphaBlend = other.m_alphaBlend;

	m_colorKey = other.m_colorKey;
}

Effect::~Effect() {
	if (m_effectSrc) {
		delete m_effectSrc;
	}
	m_effectSrc = nullptr;
}

RECT Effect::GetRC() const {
	RECT rc = { m_effectPt.x - m_effectBoxSize.cx / 2, m_effectPt.y - m_effectBoxSize.cy / 2,
		m_effectPt.x + m_effectBoxSize.cx / 2, m_effectPt.y + m_effectBoxSize.cy / 2 };

	return rc;
}

RECT Effect::GetRenderRC() const {
	RECT rc = { m_effectPt.x - m_effectBoxSize.cx / 2, m_effectPt.y - m_effectBoxSize.cy / 2,
		m_effectPt.x + m_effectBoxSize.cx / 2, m_effectPt.y + m_effectBoxSize.cy / 2 };

	return rc;
}

bool Effect::HaveClip() const {
	if (!m_effectClip) {
		return false;
	}

	return true;
}

void Effect::SetRev(bool rev) {
	m_revPrint = rev;
}

bool Effect::RevPrint() const {
	return m_revPrint;
}

void Effect::SetRotateAngle(float angle) {
	m_angle = angle;
	m_rotate = true;
}

bool Effect::RotatePrint() const {
	return m_rotate;
}

void Effect::AddNewClip(const string& strKey, const POINT& startPt, int frameLength, float frameTime) {
	m_effectSrc->AddNewClip(strKey, startPt, frameLength, frameTime);
	m_effectLimitTime = m_effectSrc->GetClip(strKey)->GetLoopTime();
}

void Effect::SetEffectInfo(const POINT& effectPt, const SIZE& effectSize,
	const SIZE& effectBoxSize, int effectLoopTime, bool moveEffect) {
	m_effectPt = effectPt;
	m_effectImgSize = effectSize;
	m_effectBoxSize = effectBoxSize;
	m_effectLoopTime = effectLoopTime;
}

void Effect::SetClip(const string& strKey) {
	if (m_effectClip) {
		m_effectClip->Reset();
	}

	m_effectClip = m_effectSrc->GetClip(strKey);
}

bool Effect::EffectEnd() {
	if (m_effectTime >= m_effectLimitTime) {
		m_effectLoop++;
		m_effectTime = 0.f;
		if (m_effectLoop >= m_effectLoopTime) {
			return true;
		}
	}

	return false;
}

void Effect::End() {
	m_effectTime = m_effectLimitTime + 0.1f;
	m_effectLoop = m_effectLoopTime;
}

void Effect::Reset() {
	m_effectTime = 0.f;
	m_effectLoopTime = 0;
	m_effectClip->Reset();
}

void Effect::SetClipNull() {
	if (m_effectClip) {
		m_effectClip->Reset();
	}
	m_effectTime = 0.f;
	m_effectLoop = 0;
	m_effectClip = nullptr;
}

void Effect::SetColorKey(COLORREF colorKey) {
	m_colorKey = colorKey;
}

void Effect::SetPt(const POINT& pt) {
	m_effectPt = pt;
}

void Effect::Update(float deltaTime) {
	m_effectTime += deltaTime;
	if (m_effectClip) {
		m_effectClip->Update(deltaTime);
	}
}

void Effect::Render(HDC destDC) {
	m_effectSrc->Render(destDC, m_effectImgSize, GetRC(),
		m_effectClip->GetImgRC(), false, m_colorKey, m_alphaBlend);
}

void Effect::Render(HDC destDC, bool rev) {
	bool reverse = rev ? true : false;

	m_effectSrc->Render(destDC, m_effectImgSize, GetRC(),
		m_effectClip->GetImgRC(), reverse, m_colorKey, m_alphaBlend);
}

void Effect::RenderRotate(HDC destDC) {
	m_effectSrc->RenderRotate(destDC, GetRenderRC(), m_effectClip->GetImgRC(), m_effectClip->GetFrameSize(),
		m_angle, m_colorKey, m_alphaBlend);
}
