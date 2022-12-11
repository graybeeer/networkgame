#include "Boss.h"
#include "EffectManager.h"

Boss::Boss(const POINT& pt) {
	AnimationManager::GetInst()->AddNewAnimation("BossAnimation", L"StoneGolem.png", POINT{ 10, 9 });

	m_animation = AnimationManager::GetInst()->GetAnimation("BossAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 4, 0.25f);
	m_animation->AddNewClip("ready", POINT{ 0, 1 }, 8, 0.2f);
	m_animation->AddNewClip("fireArm", POINT{ 0, 2 }, 9, 0.1f);
	m_animation->AddNewClip("defence", POINT{ 0, 3 }, 8, 0.1f);
	m_animation->AddNewClip("defencing", POINT{ 6, 3 }, 2, 1.f);
	m_animation->AddNewClip("attackGround", POINT{ 0, 4 }, 7, 0.2f);
	m_animation->AddNewClip("chargingLazer", POINT{ 0, 5 }, 7, 3.5f / 7.f);
	m_animation->AddNewClip("fireLazer", POINT{ 0, 5 }, 1, 2.5f);
	m_animation->AddNewClip("die", POINT{ 0, 7 }, 14, 0.1f);

	m_curClip = m_animation->GetClip("default");

	m_pt = pt;

	SIZE imgSize = m_curClip->GetFrameSize();
	float sizeScale = m_size.cx / (float)imgSize.cx;
	m_hitBoxSize = { (LONG)(m_hitBoxSizeOrigin.cx * sizeScale), (LONG)(m_hitBoxSizeOrigin.cy * sizeScale) };
	m_hitBoxSizeOrigin = m_hitBoxSize;
}

Boss::~Boss() {
	if (m_animation) {
		delete m_animation;
	}
	m_animation = nullptr;
}

RECT Boss::GetRC() const {
	RECT rc{ m_pt.x - m_hitBoxSize.cx / 2, m_pt.y - m_hitBoxSize.cy / 2,
		 m_pt.x + m_hitBoxSize.cx / 2, m_pt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

RECT Boss::GetRenderRC() const {
	RECT rc{ m_pt.x - m_size.cx / 2, m_pt.y - m_size.cy / 2,
		 m_pt.x + m_size.cx / 2, m_pt.y + m_size.cy / 2 };
	return rc;
}

POINT Boss::GetPt() const {
	return m_pt;
}

SIZE Boss::GetSize() const {
	return m_size;
}

void Boss::ResetJumpPower() {
	m_power = 0;
	m_gravityTime = 0.f;
}

void Boss::SetPower(int power, bool timeClear) {
	m_power = power;
	if (timeClear) {
		m_gravityTime = 0.f;
	}
}

void Boss::OnGround(int groundTop) {
	SetPower(0, true);
	m_onGround = true;
	m_pt.y = groundTop;
}

void Boss::HDdown(int downHP) {
	m_hp -= downHP;
	if (m_hp <= 0) {
		Die();
	}
}

bool Boss::IsInvincible() const {
	return m_invincible;
}

void Boss::Invincible(float invincibleTime) {
	m_invincibleTime = 0.f;
	m_invincible = true;
	m_invincibleEndTime = invincibleTime;
}

void Boss::InvincibleEnd() {
	if (m_invincibleTime >= m_invincibleEndTime) {
		m_invincible = false;
	}
}

bool Boss::IsAlive() const {
	return m_alive;
}

void Boss::SetDefaultAnimation() {
	
}

void Boss::SetSpAnimation(bool noMove, int loopTime, const string& anName) {
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

bool Boss::CheckSpAnimationEnd(float deltaTime) {
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

void Boss::AnimationCancel() {
	if (m_skillEffect) {
		m_skillEffect->End();
	}

	m_skillEffect = nullptr;

	m_atkInfo.Reset();
	m_speed = m_speedOrigin;
	m_curClip->Reset();
	m_curClip = m_animation->GetClip("default");
	m_spAnimation = false;
	m_noMoveAnimation = false;
	m_spAnimationTime = 0.f;
	m_spAnimationLimitTime = 0.f;
	m_spAnimationLoop = 0;
	m_spAnimationLoopTime = 0;
	m_spAnimationName = "default";
}

void Boss::SpAnimationEnd() {
	if (!m_spAnimation) {
		return;
	}

	if (m_spAnimationName == "die") {
		m_alive = false;
		return;
	}

	if (m_skillEffect) {
		m_skillEffect->End();
	}
	else if (m_moveEffect) {
		m_moveEffect->End();
	}

	m_skillEffect = nullptr;

	m_dash = false;
	m_atkInfo.Reset();
	m_speed = m_speedOrigin;
	m_curClip->Reset();
	m_curClip = m_animation->GetClip("default");
	m_spAnimation = false;
	m_noMoveAnimation = false;
	m_spAnimationTime = 0.f;
	m_spAnimationLimitTime = 0.f;
	m_spAnimationLoop = 0;
	m_spAnimationLoopTime = 0;
	m_spAnimationName = "default";
}

void Boss::Gravity(float deltaTime) {
	if (m_onGround and m_flying) {
		return;
	}

	m_gravityTime += deltaTime * m_timeScale;
	m_power -= static_cast<int>(gravity * m_gravityTime * m_gravityTime * m_timeScale);
	if (m_power <= m_limitPower) {
		m_power = static_cast<int>(m_limitPower);
	}

	m_prevPt = m_pt;
	int dist = static_cast<int>(floor(m_power * deltaTime * m_timeScale));

	m_pt.y -= dist;
	if (m_pt.y + m_hitBoxSize.cy / 2 > worldSize.cy) {
		SetPower(0);
		m_pt.y = worldSize.cy - m_hitBoxSize.cy / 2;
	}
}

void Boss::UpdateEffectPt() {
	if (m_moveEffect) {
		m_moveEffect->SetPt(m_pt);
	}
}

void Boss::AttackGround(float deltaTime) {
	if (m_spAnimationName != "attackGround" and !m_stPattern) {
		SetSpAnimation(true, 1, "attackGround");
		m_stPattern = true;
	}

	if (CheckSpAnimationEnd(deltaTime)) {
		SpAnimationEnd();
		m_atkInfo.attackStPt = { m_pt.x, m_pt.y + m_hitBoxSize.cy / 2 };
		m_atkInfo.attackBoxSize = { 100, 100 };
	}
}

void Boss::FireBullet(float deltaTime) {
	if (m_spAnimationName != "defence" and !m_stPattern) {
		m_stPattern = true;
		SetSpAnimation(true, 1, "defence");
	}

	if (CheckSpAnimationEnd(deltaTime)) {
		m_patternReadyEnd = true;
		SpAnimationEnd();
		if (m_spAnimationName != "defencing") {
			SetSpAnimation(true, 5, "defencing");
			m_fireTime = 2.5f;
		}
	}

	if (m_patternReadyEnd) {
		m_fireTime += deltaTime;
		if (m_fireLimitTime <= m_fireTime) {
			int num = GetRandInt(8, 10);
			float angle{ (float)Deg2Rad(360) };
			float sum{ (float)Deg2Rad(180) / num };
			for (int i = 0; i <= num; ++i) {
				float a = angle + sum * i;
				CreateBulletStage("Boss", POINT{ m_pt.x, m_pt.y - 100 }, MD_ANGLE, a);
			}
			m_fireTime = 0.f;
		}

		if (CheckSpAnimationEnd(deltaTime)) {
			SpAnimationEnd();
			m_fireTime = 0.f;
			m_patternReadyEnd = false;
			m_stPattern = false;
			m_ai.StateEnd();
			m_ai.m_curState = BossState::ST_DEFAULT;
		}	
	}
}

void Boss::AttackLazer(float deltaTime) {
	if (m_spAnimationName != "chargingLazer" and !m_stPattern) {
		m_stPattern = true;
		m_warning = true;
		m_targeting = true;
		SetSpAnimation(true, 1, "chargingLazer");
	}

	if (CheckSpAnimationEnd(deltaTime)) {
		m_patternReadyEnd = true;
		SpAnimationEnd();
		if (m_spAnimationName != "chargingLazer") {
			SetSpAnimation(true, 2, "chargingLazer");
			m_fireTime = 2.5f;
		}
	}

	if (m_patternReadyEnd) {
		m_fireTime += deltaTime;
		if (1.f <= m_fireTime) {
			POINT centerPt{ m_pt.x, m_pt.y - 120 };
			CreateBulletStage("Boss", centerPt, MD_TARGET);
			m_fireTime = 0.f;
		}

		if (CheckSpAnimationEnd(deltaTime)) {
			SpAnimationEnd();
			m_fireTime = 0.f;
			m_patternReadyEnd = false;
			m_stPattern = false;
			m_warning = false;
			m_targeting = false;
		}
	}
}

void Boss::Die() {
	m_deadAnimation = true;
	AnimationCancel();
	SetSpAnimation(true, 1, "die");
}

void Boss::Update(float deltaTime, const POINT& playerPt) {
	if (m_invincible) {
		m_invincibleTime += deltaTime;
		InvincibleEnd();
	}
	
	m_curClip->Update(deltaTime);

	if (m_targeting) {
		m_playerPt = playerPt;
	}

	if (!m_flying) {
		Gravity(deltaTime);
	}

	float width = (static_cast<float>(m_playerPt.x - m_pt.x));
	float height = (static_cast<float>(m_playerPt.y - m_pt.y));

	m_angle = atan2(height, width);

	static bool once = false;
	if (!once) {
		m_barriorEffect = EffectManager::GetInst()->CreateEffectInWorld("CreateShieldEffect", SIZE{ 500, 500 }, m_pt, 1);
		once = true;
	}

	static bool once2 = false;
	if (m_barriorEffect) {
		if (m_barriorEffect->EffectEnd() and !once2) {
			EffectManager::GetInst()->CreateEffectInWorld("ShieldEffect", SIZE{ 500, 500 }, m_pt, 5);
			once2 = true;
		}
		m_barriorEffect = nullptr;
	}

	if (m_ai.m_curState == BossState::ST_DEFAULT) {
		m_ai.Update(deltaTime);
	}

	if (m_ai.m_curState == BossState::ST_PATTERN) {
		switch (m_ai.m_patternNum) {
		case 0:
			FireBullet(deltaTime);
			break;

		case 1:
			AttackLazer(deltaTime);
			break;
		}
	}

	m_onGround = false;

	if (m_ai.m_curState == BossState::ST_DEFAULT) {
		if (CheckSpAnimationEnd(deltaTime)) {
			SpAnimationEnd();
		}
	}

	/*CheckOut();*/
}

void Boss::Render(HDC destDC) {
	RenderHitBox(destDC, GetRC());

	bool rev = m_playerPt.x < m_pt.x ? true : false;
	//m_animation->Render(destDC, m_size, GetRenderRC(), m_curClip->GetImgRC(), rev);

	RECT rc{ m_curClip->GetImgRC() };
	SIZE sz{ m_curClip->GetFrameSize() };

	CImage img{ };
	img.Create(sz.cx, sz.cy, 32);
	StretchBlt(img.GetDC(), rev ? sz.cx : 0, 0, rev ? -sz.cx : sz.cx, sz.cy, m_animation->GetDC(),
		rc.left, rc.top, sz.cx, sz.cy, SRCCOPY);
	img.ReleaseDC();
	m_animation->ReleaseDC();

	img.TransparentBlt(destDC, m_pt.x - 400, m_pt.y - 400, 800, 800, 0, 0,
		sz.cx, sz.cy, RGB(0, 0, 0));

	if (m_warning) {
		HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(destDC, hPen);

		MoveToEx(destDC, m_pt.x, m_pt.y - 100, nullptr);
		LineTo(destDC, m_playerPt.x, m_playerPt.y);

		hPen = (HPEN)SelectObject(destDC, hOldPen);
		DeleteObject(hPen);
	}
}
