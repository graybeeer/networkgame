#include "Button.h"


Button::Button() {
	AnimationManager::GetInst()->AddNewAnimation("ButtonAnimation", L"Button.bmp", POINT{ 2, 1 });

	m_animation = AnimationManager::GetInst()->GetAnimation("ButtonAnimation");
	m_animation->AddNewClip("disable", POINT{ 0, 0 }, 1, 1.f);
	m_animation->AddNewClip("enable", POINT{ 1, 0 }, 1, 1.f);

	m_curClip = m_animation->GetClip("disable");
}

Button::Button(const POINT startPt) {
	m_buttonPt = startPt;
	AnimationManager::GetInst()->AddNewAnimation("ButtonAnimation", L"Button.bmp", POINT{ 2, 1 });

	m_animation = AnimationManager::GetInst()->GetAnimation("ButtonAnimation");
	m_animation->AddNewClip("disable", POINT{ 0, 0 }, 1, 1.f);
	m_animation->AddNewClip("enable", POINT{ 1, 0 }, 1, 1.f);

	m_curClip = m_animation->GetClip("disable");
}

Button::~Button() {
	if (m_animation) {
		delete m_animation;
	}
	m_animation = nullptr;
}

POINT Button::GetButtonPt() const {
	return m_buttonPt;
}

SIZE Button::GetButtonSize() const {
	return m_buttonSize;
}

RECT Button::GetButtonRC() const {
	RECT rc{ m_buttonPt.x - m_hitBoxSize.cx / 2, m_buttonPt.y - m_hitBoxSize.cy / 2,
			m_buttonPt.x + m_hitBoxSize.cx / 2, m_buttonPt.y + m_hitBoxSize.cy / 2 };

	return rc;
}

POINT Button::GetPrevPt() const {
	return m_prevPt;
}

RECT Button::GetRenderRC() const {
	return RECT{ m_buttonPt.x - m_buttonSize.cx / 2, m_buttonPt.y - m_buttonSize.cy / 2,
		m_buttonPt.x + m_buttonSize.cx / 2, m_buttonPt.y + m_buttonSize.cy / 2 };
}

RECT Button::GetPrevRC() const {
	RECT rc{ m_prevPt.x - m_hitBoxSize.cx / 2, m_prevPt.y - m_hitBoxSize.cy / 2,
		m_prevPt.x + m_hitBoxSize.cx / 2, m_prevPt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

void Button::SetPt(const POINT& pt) {
	m_buttonPt = pt;
}

void Button::OnGround(int groundTop) {
	m_onGround = true;
	m_buttonPt.y = groundTop - m_hitBoxSize.cy / 2;
	m_power = 0.f;
	m_gravityTime = 0.f;
}

void Button::OnGround() {
	m_onGround = true;
}


bool Button::IsInvincible() const {
	return m_invincible;
}

bool Button::Diying() const {
	return m_deadAimation;
}

void Button::Attacked(int attackDamage, int dir, float attackTime, int knockBackPower) {
	if (m_deadAimation) {
		return;
	}
	
	Die();
}

void Button::Invincible(float invincibleTime) {
	m_invincibleTime = 0.f;
	m_invincibleEndTime = invincibleTime;
	m_invincible = true;
}

void Button::InvincibleEnd() {
	if (m_invincibleEndTime <= m_invincibleTime) {
		m_invincible = false;
	}
}

void Button::Update(float deltaTime) {
	m_curClip->Update(deltaTime);
	if (m_invincible) {
		m_invincibleTime += deltaTime;
		InvincibleEnd();
	}

	if (!m_alive) {
		m_time += deltaTime;
		if (m_time >= m_reviveTime) {
			Revive();
			m_time = 0.f;
		}
	}

	if (CheckSpAnimationEnd(deltaTime)) {
		SpAnimationEnd();
	}
}

void Button::Render(HDC destDC) {
	if (m_buttonPt.x - m_hitBoxSize.cx / 2 > cameraPos.x + clientSize.cx / 2) {
		return;
	}
	else if (m_buttonPt.x + m_hitBoxSize.cx / 2 < cameraPos.x - clientSize.cx / 2) {
		return;
	}

	if (m_buttonPt.y - m_hitBoxSize.cy / 2 > cameraPos.y + clientSize.cy / 2) {
		return;
	}
	else if (m_buttonPt.y + m_hitBoxSize.cy / 2 < cameraPos.y - clientSize.cy / 2) {
		return;
	}

	RenderHitBox(destDC, GetButtonRC());
	m_animation->Render(destDC, GetRenderRC(), m_curClip->GetImgRC(), RGB(255, 255, 255));
}

void Button::SetDefaultAnimation() {
	m_curClip->Reset();
	m_curClip = m_animation->GetClip("disable");
	m_spAnimation = false;
	m_noMoveAnimation = false;
	m_spAnimationLimitTime = m_curClip->GetLoopTime();
}

void Button::SetSpAnimation(bool noMove, int loopTime, const string& anName) {
	if (m_spAnimation) {
		return;
	}

	m_curClip->Reset();
	m_curClip = m_animation->GetClip(anName);
	m_spAnimation = true;
	m_noMoveAnimation = noMove;
	m_spAnimationTime = 0.f;
	m_spAnimationLimitTime = m_curClip->GetLoopTime();
	m_spAnimationLoop = 0;
	m_spAnimationLoopTime = loopTime;
	m_spAnimationName = anName;
}

bool Button::CheckSpAnimationEnd(float deltaTime) {
	if (m_spAnimation) {
		m_spAnimationTime += deltaTime;
		if (m_spAnimationLimitTime <= m_spAnimationTime) {
			m_spAnimationLoop++;
			m_spAnimationTime = 0.f;
			if (m_spAnimationLoop == m_spAnimationLoopTime) {
				return true;
			}
		}
	}

	return false;
}

void Button::SpAnimationEnd() {
	if (!m_spAnimation) {
		return;
	}

	if (m_spAnimationName == "enable") {
		m_alive = false;
		return;
	}

	m_curClip->Reset();
	m_curClip = m_animation->GetClip("enable");
	m_spAnimation = false;
	m_noMoveAnimation = false;
	m_spAnimationTime = 0.f;
	m_spAnimationLimitTime = 0.f;
	m_spAnimationLoop = 0;
	m_spAnimationLoopTime = 0;
	m_spAnimationName = "enable";
}

void Button::Die() {
	m_deadAimation = true;
	m_alive = false;
	m_hitBoxSize = { 60, 10 };
	m_buttonPt.y += 10;
	SetSpAnimation(false, 0, "enable");
}

void Button::Revive() {
	m_deadAimation = false;
	m_alive = true;
	m_hitBoxSize = { 60, 20 };
	m_buttonPt.y -= 10;
	SetDefaultAnimation();
}

bool Button::Save(FILE* fp)
{
	fwrite(&m_buttonPt, sizeof(POINT), 1, fp);
	return true;
}

bool Button::Load(FILE* fp)
{
	fread(&m_buttonPt, sizeof(POINT), 1, fp);
	return true;
}
