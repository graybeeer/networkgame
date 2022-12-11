#include "Monster.h"
#include "AnimationManager.h"
#include "EffectManager.h"
#include "Global.h"

Monster::Monster() {
}

Monster::Monster(const POINT startPt) {
	m_monsterPt = startPt;
}

Monster::Monster(const POINT startPt, int moveDir) {
	m_monsterPt = startPt;
	m_moveDir = moveDir;
}

Monster::~Monster() {
	if (m_animation) {
		delete m_animation;
	}
	m_animation = nullptr;
}

void Monster::SetColorKey(const COLORREF& colorKey) {
	m_colorKey = colorKey;
}

POINT Monster::GetMonsterPt() const {
	return m_monsterPt;
}

SIZE Monster::GetMonsterSize() const {
	return m_monsterSize;
}

RECT Monster::GetMonsterRC() const {
	RECT rc{ m_monsterPt.x - m_hitBoxSize.cx / 2, m_monsterPt.y - m_hitBoxSize.cy / 2,
			m_monsterPt.x + m_hitBoxSize.cx / 2, m_monsterPt.y + m_hitBoxSize.cy / 2 };

	return rc;
}

POINT Monster::GetPrevPt() const {
	return m_prevPt;
}

RECT Monster::GetRenderRC() const {
	return RECT{ m_monsterPt.x - m_monsterSize.cx / 2, m_monsterPt.y - m_monsterSize.cy / 2,
		m_monsterPt.x + m_monsterSize.cx / 2, m_monsterPt.y + m_monsterSize.cy / 2 };
}

RECT Monster::GetPrevRC() const {
	RECT rc{ m_prevPt.x - m_hitBoxSize.cx / 2, m_prevPt.y - m_hitBoxSize.cy / 2,
		m_prevPt.x + m_hitBoxSize.cx / 2, m_prevPt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

const string& Monster::GetTag() const {
	return m_tag;
}

void Monster::SetPt(const POINT& pt) {
	m_monsterPt = pt;
}

void Monster::OnGround(int groundTop, const RECT& groundRC) {
	m_onGround = true;
	m_monsterPt.y = groundTop - m_hitBoxSize.cy / 2;
	m_power = 0.f;
	m_gravityTime = 0.f;
	m_jumped = false;
	m_groundRC = groundRC;
}

void Monster::OnGround() {
	m_onGround = true;
}

bool Monster::IsAlive() const {
	return m_alive;
}

bool Monster::IsInvincible() const {
	return m_invincible;
}

bool Monster::Diying() const {
	return m_deadAimation;
}

void Monster::Gravity(float deltaTime) {
	if (m_onGround and !m_jumped) {
		return;
	}

	m_gravityTime += deltaTime * 1.5f;
	m_power -= gravity * m_gravityTime * m_gravityTime * 1.5f;
	if (m_limitPower <= m_power) {
		m_power = m_limitPower;
	}

	m_prevPt = m_monsterPt;
	m_monsterPt.y -= static_cast<int>(floor(m_power * deltaTime * 1.5f));
	if (m_monsterPt.y + m_hitBoxSize.cy / 2 > worldSize.cy) {
		m_monsterPt.y = worldSize.cy - m_hitBoxSize.cy / 2;
		m_jumped = false;
		m_gravityTime = 0.f;
		m_power = 0.f;
	}
}

void Monster::Attacked(int attackDamage, int dir, float attackTime, int knockBackPower) {
	if (m_deadAimation) {
		return;
	}

	if (m_spAnimationName != "attacked") {
		SetDefaultAnimation();
	}

	m_knockBack = true;
	m_hp -= attackDamage;
	if (m_hp <= 0) {
		m_spAnimation = false;
		Die();
		return;
	}

	m_lookLR = m_moveDir = dir < 0 ? MD_RIGHT : MD_LEFT;

	m_knockBackPower = knockBackPower;
	m_knockBackDist = 0;
	if (m_knockBackPower < 0) {
		m_knockBackPower = 0;
	}

	m_knockBackDir = dir;

	if (attackTime <= 0.f) {
		attackTime = 1.f;
	}

	Invincible(attackTime);
	SetSpAnimation(true, 1, "attacked");
}

void Monster::Attacked(ATTACKINFO& atkInfo, int dir) {
	if (m_deadAimation) {
		return;
	}

	if (m_spAnimationName != "attacked") {
		SetDefaultAnimation();
	}

	m_knockBack = true;
	m_hp -= atkInfo.attackDamage;
	if (m_hp <= 0) {
		m_spAnimation = false;
		Die();
		return;
	}

	m_lookLR = m_moveDir = dir < 0 ? MD_RIGHT : MD_LEFT;

	m_knockBackPower = atkInfo.knockBackPower;
	m_knockBackDist = 0;
	if (m_knockBackPower < 0) {
		m_knockBackPower = 0;
	}

	m_knockBackDir = dir;

	if (atkInfo.attackTime <= 0.f) {
		atkInfo.attackTime = 1.f;
	}

	if (atkInfo.attackedEffectTag != "") {
		int x = GetRandInt(-m_hitBoxSize.cy / 2, m_hitBoxSize.cy / 2);
		int y = GetRandInt(-m_hitBoxSize.cx / 2, m_hitBoxSize.cx / 2);
		POINT effectPt{ m_monsterPt.x + x, m_monsterPt.y + y };

		EffectManager::GetInst()->CreateEffectInWorld(atkInfo.attackedEffectTag,
			atkInfo.effectSize, effectPt, 1);
	}

	Invincible(atkInfo.attackTime);
	SetSpAnimation(true, 1, "attacked");
}

void Monster::Invincible(float invincibleTime) {
	m_invincibleTime = 0.f;
	m_invincibleEndTime = invincibleTime;
	m_invincible = true;
}

void Monster::InvincibleEnd() {
	if (m_invincibleEndTime <= m_invincibleTime) {
		m_invincible = false;
	}
}

void Monster::Update(float deltaTime) {
	m_curClip->Update(deltaTime);

	if (m_invincible) {
		m_invincibleTime += deltaTime;
		InvincibleEnd();
	}

	if (!m_knockBack and !m_spAnimation) {
		switch (m_ai.Update(deltaTime, m_moveDir)) {
		case state::ST_DEFAULT:
			SetDefaultAnimation();
			break;

		case state::ST_MOVE:
			int moveDist{ };
			int randDir = GetRandInt(0, 1);

			if (!m_noMoveAnimation and !m_knockBack) {
				if (m_spAnimationName != "move") {
					m_curClip->Reset();
					m_spAnimationName = "move";
					m_curClip = m_animation->GetClip("move");
				}

				moveDist = Move(deltaTime);
				if (m_moveDir == MD_RIGHT) {
					if (!m_ai.OnGround(m_groundRC, GetMonsterRC(), moveDist)) {
						m_moveDir = MD_LEFT;
					}
				}
				else if (m_moveDir == MD_LEFT) {
					if (!m_ai.OnGround(m_groundRC, GetMonsterRC(), -moveDist)) {
						m_moveDir = MD_RIGHT;
					}
				}
			}
			break;
		}
	}

	if (m_knockBack) {
		MoveDist(deltaTime, m_knockBackPower, m_knockBackDir);
	}

	Gravity(deltaTime);
	m_onGround = false;

	if (CheckSpAnimationEnd(deltaTime)) {
		SpAnimationEnd();
	}

	m_hpBar.Update(m_hp, m_monsterPt);
}

void Monster::Render(HDC destDC) {
	if (m_monsterPt.x - m_hitBoxSize.cx / 2 > cameraPos.x + clientSize.cx / 2) {
		return;
	}
	else if (m_monsterPt.x + m_hitBoxSize.cx / 2 < cameraPos.x - clientSize.cx / 2) {
		return;
	}

	if (m_monsterPt.y - m_hitBoxSize.cy / 2 > cameraPos.y + clientSize.cy / 2) {
		return;
	}
	else if (m_monsterPt.y + m_hitBoxSize.cy / 2 < cameraPos.y - clientSize.cy / 2) {
		return;
	}

	RenderHitBox(destDC, GetMonsterRC());

	bool rev = m_lookLR == MD_RIGHT ? true : false;
	m_animation->Render(destDC, m_monsterSize, GetRenderRC(),
		m_curClip->GetImgRC(), rev, m_colorKey);

	m_hpBar.Render(destDC);
}

void Monster::Input(float deltaTime) {

}

void Monster::MoveDist(float deltaTime, int dist, int moveDir) {
	if (dist == 0 or moveDir == 0 or m_deadAimation) {
		m_knockBackPower = 0;
		m_knockBackDist = 0;
		m_knockBack = false;
		return;
	}

	int moveDist = static_cast<int>(round(m_knockBackPower * moveDir * deltaTime * m_knockBackScale));
	m_knockBackDist += abs(moveDist);
	m_monsterPt.x += moveDist;
	if (m_monsterPt.x - m_hitBoxSize.cx / 2 < 0) {
		m_monsterPt.x = m_hitBoxSize.cx;
	}
	else if (m_monsterPt.x + m_hitBoxSize.cx / 2 > worldSize.cx) {
		m_monsterPt.x = worldSize.cx - m_hitBoxSize.cx / 2;
	}

	if (m_knockBackDist >= dist) {
		m_knockBackDist = 0;
		m_knockBackPower = 0;
		m_knockBack = false;
	}
}

int Monster::Move(float deltaTime) {
	int moveDist{ };
	switch (m_moveDir) {
	case MD_LEFT:
		if (!m_noMoveAnimation) {
			moveDist = MoveLeft(deltaTime);
		}
		break;

	case MD_RIGHT:
		if (!m_noMoveAnimation) {
			moveDist = MoveRight(deltaTime);
		}
		break;

	case MD_UP:
		if (!m_noMoveAnimation) {
			MoveUp(deltaTime);
		}
		break;

	case MD_DOWN:
		if (!m_noMoveAnimation) {
			MoveDown(deltaTime);
		}
		break;
	}

	return moveDist;
}

int Monster::MoveLeft(float deltaTime) {
	int moveDist = static_cast<int>(round(m_speed * deltaTime));
	m_monsterPt.x -= moveDist;
	m_look = MD_LEFT;
	m_lookLR = MD_LEFT;
	if (m_monsterPt.x - m_hitBoxSize.cx / 2 <= 0) {
		m_monsterPt.x = m_hitBoxSize.cx / 2;
		m_moveDir = MD_RIGHT;
	}
	return moveDist;
}

int Monster::MoveRight(float deltaTime) {
	int moveDist = static_cast<int>(round(m_speed * deltaTime));
	m_monsterPt.x += moveDist;
	m_look = MD_RIGHT;
	m_lookLR = MD_RIGHT;
	if (m_monsterPt.x + m_hitBoxSize.cx / 2 >= worldSize.cx) {
		m_monsterPt.x = worldSize.cx - m_hitBoxSize.cx / 2;
		m_moveDir = MD_LEFT;
	}
	return moveDist;
}

void Monster::MoveUp(float deltaTime) {
	m_monsterPt.y -= static_cast<int>(round(m_speed * deltaTime));
	m_look = MD_UP;
	if (m_monsterPt.y - m_hitBoxSize.cy / 2 <= 0) {
		m_monsterPt.y = m_hitBoxSize.cy / 2;
		m_moveDir = MD_DOWN;
	}
}

void Monster::MoveDown(float deltaTime) {
	m_monsterPt.y += static_cast<int>(round(m_speed * deltaTime));
	m_look = MD_DOWN;
	if (m_monsterPt.y + m_hitBoxSize.cy / 2 >= worldSize.cy) {
		m_monsterPt.y = worldSize.cy - m_hitBoxSize.cy / 2;
		m_moveDir = MD_UP;
	}
}

void Monster::SetDefaultAnimation() {
	if (m_spAnimationName != "default") {
		m_curClip->Reset();
		m_spAnimationName = "default";
	}
	m_curClip = m_animation->GetClip("default");
}

void Monster::SetSpAnimation(bool noMove, int loopTime, const string& anName) {
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

bool Monster::CheckSpAnimationEnd(float deltaTime) {
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

void Monster::SpAnimationEnd() {
	if (!m_spAnimation) {
		return;
	}

	if (m_spAnimationName == "die") {
		m_alive = false;
		return;
	}

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

void Monster::Die() {
	m_deadAimation = true;
	SetSpAnimation(true, 1, "die");
}

bool Monster::Save(FILE* fp) {
	fwrite(&m_monsterPt, sizeof(POINT), 1, fp);
	fwrite(&m_hitBoxSize, sizeof(SIZE), 1, fp);
	fwrite(&m_monsterSize, sizeof(SIZE), 1, fp);
	return true;
}

bool Monster::Load(FILE* fp) {
	fread(&m_monsterPt, sizeof(POINT), 1, fp);
	fread(&m_hitBoxSize, sizeof(SIZE), 1, fp);
	fread(&m_monsterSize, sizeof(SIZE), 1, fp);
	return true;
}
