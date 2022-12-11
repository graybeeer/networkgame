#include "Player.h"
#include "AnimationManager.h"
#include "EffectManager.h"
#include "Server.h"

Player::Player(bool myControl, int playerNum) {
	switch(playerNum)
	{
	case 0: //플레이어 번호마다 캐릭터 스프라이트 다르게 하기
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation", L"RedHood.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation");
		break;
	case 1:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation2", L"RedHood2.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation2");
		break;
	case 2:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation3", L"RedHood3.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation3");
		break;
	case 3:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation4", L"RedHood4.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation4");
		break;
	default:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation", L"RedHood.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation");
		break;
	}

	

	m_playerSize = { 100, 100 };

	// AnimationClip 설정
	m_animation->AddNewClip("default", POINT{ 0 ,0 }, 18, 0.1f);
	m_animation->AddNewClip("run", POINT{ 0, 1 }, 24, 0.03f);
	m_animation->AddNewClip("jump", POINT{ 0, 2 }, 19, 0.05f);
	m_animation->AddNewClip("lightAtk", POINT{ 5, 3 }, 7, 0.05f);
	m_animation->AddNewClip("fastAtk", POINT{ 0, 3 }, 26, 0.01f);
	m_animation->AddNewClip("hit", POINT{ 0, 4 }, 7, 0.1f);
	m_animation->AddNewClip("dash", POINT{ 0, 4 }, 7, 0.03f);
	m_animation->AddNewClip("AxeAtk", POINT{ 5, 3 }, 7, 0.11f);
	m_animation->AddNewClip("die", POINT{ 0, 4 }, 3, 0.1f);

	m_curClip = m_animation->GetClip("default");

	SIZE imgSize = m_curClip->GetFrameSize();
	float sizeScale = m_playerSize.cx / (float)imgSize.cx;
	m_hitBoxSize = { (LONG)(m_hitBoxSizeOrigin.cx * sizeScale), (LONG)(m_hitBoxSizeOrigin.cy * sizeScale) };
	m_hitBoxSizeOrigin = m_hitBoxSize;

	//내가 조정하는 캐릭터로 생성되면 해당 bool 변수 true
	if (myControl)
		m_myControl = true;
}

Player::Player(bool myControl, int playerNum, const POINT& pt) {
	switch (playerNum)
	{
	case 0: //플레이어 번호마다 캐릭터 스프라이트 다르게 하기
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation", L"RedHood.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation");
		break;
	case 1:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation2", L"RedHood2.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation2");
		break;
	case 2:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation3", L"RedHood3.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation3");
		break;
	case 3:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation4", L"RedHood4.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation4");
		break;
	default:
		AnimationManager::GetInst()->AddNewAnimation("PlayerAnimation", L"RedHood.bmp", POINT{ 26, 5 });
		m_animation = AnimationManager::GetInst()->GetAnimation("PlayerAnimation");
		break;
	}
	

	m_playerPt = pt;

	m_playerSize = { 100, 100 };

	// AnimationClip 설정
	m_animation->AddNewClip("default", POINT{ 0 ,0 }, 18, 0.1f);
	m_animation->AddNewClip("run", POINT{ 0, 1 }, 24, 0.03f);
	m_animation->AddNewClip("jump", POINT{ 0, 2 }, 19, 0.05f);
	m_animation->AddNewClip("lightAtk", POINT{ 5, 3 }, 7, 0.05f);
	m_animation->AddNewClip("fastAtk", POINT{ 0, 3 }, 26, 0.01f);
	m_animation->AddNewClip("hit", POINT{ 0, 4 }, 7, 0.1f);
	m_animation->AddNewClip("AxeAtk", POINT{ 5, 3 }, 7, 0.11f);
	m_animation->AddNewClip("die", POINT{ 0, 4 }, 3, 0.1f);

	m_curClip = m_animation->GetClip("default");

	SIZE imgSize = m_curClip->GetFrameSize();
	float sizeScale = m_playerSize.cx / (float)imgSize.cx;
	m_hitBoxSize = { (LONG)(m_hitBoxSizeOrigin.cx * sizeScale), (LONG)(m_hitBoxSizeOrigin.cy * sizeScale) };
	m_hitBoxSizeOrigin = m_hitBoxSize;

	//내가 조정하는 캐릭터로 생성되면 해당 bool 변수 true
	if (myControl)
		m_myControl = true;
}

Player::~Player() {
	if (m_animation) {
		delete m_animation;
	}
	m_animation = nullptr;
}

POINT Player::GetPlayerPt() const {
	return m_playerPt;
}

SIZE Player::GetPlayerSize() const {
	return m_playerSize;
}

RECT Player::GetPlayerRC() const {
	if (m_spAnimationName == "die") {
		return RECT{ -1, -1, 0, 0 };
	}
	RECT rc{ m_playerPt.x - m_hitBoxSize.cx / 2, m_playerPt.y - m_hitBoxSize.cy / 2,
		m_playerPt.x + m_hitBoxSize.cx / 2, m_playerPt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

POINT Player::GetPrevPt() const {
	return m_prevPt;
}

SIZE Player::GetHitBoxSize() const {
	return m_hitBoxSize;
}

RECT Player::GetRenderRC() const {
	return RECT{ m_playerPt.x - m_playerSize.cx / 2, m_playerPt.y - m_playerSize.cy / 2,
		m_playerPt.x + m_playerSize.cx / 2, m_playerPt.y + m_playerSize.cy / 2 };
}

int Player::GetKnockBackPower() const {
	return m_knockBackPower;
}

ATTACKINFO Player::GetAttackInfo() const {
	return m_atkInfo;
}

int Player::GetHP() const {
	return m_hp;
}

RECT Player::GetPrevRC() const {
	RECT rc{ m_prevPt.x - m_hitBoxSize.cx / 2, m_prevPt.y - m_hitBoxSize.cy / 2,
		m_prevPt.x + m_hitBoxSize.cx / 2, m_prevPt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

void Player::ResetJumpPower() {
	m_power = static_cast<int>(m_jumpPowerOrigin);
	m_gravityTime = 0.f;
}

void Player::SetPower(int power, bool timeClear) {
	m_power = power;
	if (timeClear) {
		m_gravityTime = 0.f;
	}
}

void Player::OnGround(int grountTop) {
	m_onGround = true;
	m_playerPt.y = grountTop - (m_hitBoxSize.cy / 2);
	if (m_jumped) {
		m_jumped = false;
		if (!m_spAnimation) {
			m_curClip->Reset();
			m_curClip = m_animation->GetClip("default");
		}
	}
	SetPower(0, true);
}

void Player::OnGround() {
	m_onGround = true;
}

void Player::SetX(int x) {
	m_playerPt.x = x;
}

void Player::SetPt(const POINT& pt) {
	m_playerPt = pt;
}

void Player::HDdown(int downHP) {
	m_hp -= downHP;
	if (m_hp < 1000) {
		Die();
	}
}

bool Player::IsInvincible() const {
	return m_invincible;
}

bool Player::IsAlive() const {
	return m_alive;
}

void Player::Invincible(float invincibleTime) {
	m_invincibleTime = 0.f;
	m_invincibleEndTime = invincibleTime;
	m_invincible = true;
}

void Player::InvincibleEnd() {
	if (m_invincibleEndTime <= m_invincibleTime) {
		m_invincible = false;
	}
}

void Player::Update(float deltaTime) {
	if (m_invincible) {
		m_invincibleTime += deltaTime;
		InvincibleEnd();
	}

	if (m_knockBack) {
		MoveDist(deltaTime, m_knockBackPower, m_knockBackDir);
	}

	if (m_spAnimationName != "dash") {
		m_speed = m_speedOrigin;
		m_dash = false;
	}

	Gravity(deltaTime);

	m_onGround = false;

	m_curClip->Update(deltaTime);

	if (CheckSpAnimationEnd(deltaTime)) {
		SpAnimationEnd();
	}

	m_hpBar.Update(m_hp, m_playerPt);

	UpdateEffectPt();

	CheckOut();
}

void Player::Render(HDC destDC) {
	RenderHitBox(destDC, GetPlayerRC());
	RenderHitBox(destDC, m_atkInfo.attackBox);
	
	bool rev = m_lookLR == MD_LEFT ? true : false;
	m_animation->Render(destDC, m_playerSize, GetRenderRC(), m_curClip->GetImgRC(), rev, RGB(255, 255, 255));

	m_hpBar.Render(destDC);

}

void Player::Input(float deltaTime) {
	if (m_myControl)
	{
		if (m_deadAnimation) {
			return;
		}

		if (!m_noMoveAnimation) {
			if (!m_dash) {
				if (keyInfo[VK_LEFT].dClick or keyInfo[VK_RIGHT].dClick) {
					Dash();
				}
			}

			if (keyInfo[VK_LEFT].pressed) {
				if (!m_spAnimation and !m_jumped) {
					m_curClip = m_animation->GetClip("run");
				}
				MoveLeft(deltaTime);
			}
			else if (keyInfo[VK_LEFT].up) {
				if (!m_spAnimation and !m_jumped) {
					m_curClip = m_animation->GetClip("default");
				}
			}

			if (keyInfo[VK_RIGHT].pressed) {
				if (!m_spAnimation and !m_jumped) {
					m_curClip = m_animation->GetClip("run");
				}
				MoveRight(deltaTime);
			}
			else if (keyInfo[VK_RIGHT].up) {
				if (!m_spAnimation and !m_jumped) {
					m_curClip = m_animation->GetClip("default");
				}
			}

			if (debuging) {
				if (keyInfo[VK_UP].pressed) {
					if (!m_jumped) {
						if (!m_spAnimation) {
							m_curClip = m_animation->GetClip("run");
						}
						SetPower(0, true);
						MoveUp(deltaTime);
					}
				}
				else if (keyInfo[VK_UP].up) {
					if (!m_spAnimation) {
						m_curClip = m_animation->GetClip("default");
					}
				}
			}
		}

		if (debuging) {
			if (keyInfo[VK_DOWN].pressed) {
				if (!m_jumped) {
					if (!m_spAnimation) {
						m_curClip = m_animation->GetClip("run");
					}
					SetPower(0, true);
					MoveDown(deltaTime);
				}
			}
			else if (keyInfo[VK_DOWN].up) {
				if (!m_spAnimation) {
					m_curClip = m_animation->GetClip("default");
				}
			}
		}

		if (keyInfo[VK_SPACE].down) {
			if (!m_jumped) {
				m_jumped = true;
				if (!m_spAnimation) {
					m_curClip->Reset();
					m_curClip = m_animation->GetClip("jump");
				}
				Jump();
			}
		}

		if (m_dash) {
			Move(deltaTime, m_lookLR);
		}

		if (keyInfo['A'].down) {
			Skill1();
		}

		if (keyInfo['Z'].down) {
			Attack();
		}

		if (keyInfo['X'].dClick) {
			FastAttack();
		}

		if (keyInfo['C'].down) {
			SetSpAnimation(true, 1, "AxeAtk");
		}
	}
}

void Player::MoveDist(float deltaTime, int dist, int moveDir) {
	if (dist == 0 or moveDir == 0 or m_deadAnimation) {
		m_knockBackPower = 0;
		m_knockBackDist = 0;
		m_knockBack = false;
		return;
	}

	int moveDist = static_cast<int>(round(m_knockBackPower * moveDir * deltaTime * m_knockBackScale));
	m_knockBackDist += abs(moveDist);
	m_playerPt.x += moveDist;
	if (m_playerPt.x - m_hitBoxSize.cx / 2 < 0) {
		m_playerPt.x = m_hitBoxSize.cx;
	}
	else if (m_playerPt.x + m_hitBoxSize.cx / 2 > worldSize.cx) {
		m_playerPt.x = worldSize.cx - m_hitBoxSize.cx / 2;
	}

	if (m_knockBackDist >= dist) {
		m_knockBackDist = 0;
		m_knockBackPower = 0;
		m_knockBack = false;
		SetDefaultAnimation();
	}
}

void Player::Move(float deltaTime, int moveDir) {
	switch (moveDir) {
	case MD_LEFT:
		MoveLeft(deltaTime);
		break;

	case MD_RIGHT:
		MoveRight(deltaTime);
		break;
	}
}

void Player::MoveLeft(float deltaTime) {
	m_playerPt.x -= static_cast<int>(m_speed * deltaTime);
	m_look = MD_LEFT;
	m_lookLR = m_look;
}

void Player::MoveRight(float deltaTime) {
	m_playerPt.x += static_cast<int>(m_speed * deltaTime);
	m_look = MD_RIGHT;
	m_lookLR = m_look;
}

void Player::MoveUp(float deltaTime) {
	m_playerPt.y -= static_cast<int>(m_speed * deltaTime);
	if (m_look != MD_UP && m_look != MD_DOWN) {
		m_lookLR = m_look;
	}
	m_look = MD_UP;
}

void Player::MoveDown(float deltaTime) {
	m_playerPt.y += static_cast<int>(m_speed * deltaTime);
	if (m_look != MD_UP && m_look != MD_DOWN) {
		m_lookLR = m_look;
	}
	m_look = MD_DOWN;
}

void Player::CheckOut() {
	if (m_playerPt.x + m_hitBoxSize.cx / 2 > worldSize.cx) {
		m_playerPt.x = worldSize.cx - (m_hitBoxSize.cx / 2);
	}
	else if (m_playerPt.x - m_hitBoxSize.cx / 2 <= 0) {
		m_playerPt.x = m_hitBoxSize.cx / 2;
	}
}

void Player::Attacked(int attackDamage, int dir, int knockBackPower) {
	if (m_deadAnimation) {
		return;
	}

	if (m_spAnimationName != "dash") {
		AnimationCancel();
	}

	m_knockBack = true;
	m_hp -= attackDamage;
	if (m_hp <= 0) {
		m_spAnimation = false;
		Die();
		return;
	}

	m_knockBackPower = knockBackPower;
	m_knockBackDist = 0;
	if (m_knockBackPower == 0) {
		m_knockBackPower = 50;
	}

	m_knockBackDir = dir;

	Invincible(1.f);
	SetSpAnimation(true, 1, "hit");
}

void Player::Dash() {
	if (m_spAnimation) {
		return;
	}

	m_dash = true;
	Invincible(m_animation->GetClip("dash")->GetLoopTime());
	SetSpAnimation(true, 1, "dash");
	m_speed = 1000.f;
}

void Player::Skill1() {
	if (m_spAnimation) {
		return;
	}
}

void Player::FastAttack() {
	if (m_spAnimation or m_spAnimationName == "hit") {
		return;
	}

	int loopTime = 3;

	int dir = m_lookLR == MD_LEFT ? -1 : 1;
	POINT effectPt{ m_playerPt.x + (dir * m_playerSize.cx / 2), m_playerPt.y };
	m_skillEffect = EffectManager::GetInst()->CreateEffectInWorld("Effect002", SIZE{ 100, 100 }, effectPt, 1);
	if (dir < 0) {
		m_skillEffect->SetRev(true);
	}

	if (dir < 0) {
		POINT lt{ m_playerPt.x - 75, m_playerPt.y - m_hitBoxSize.cy / 2 };
		POINT rb{ m_playerPt.x, m_playerPt.y + m_hitBoxSize.cy / 2 };
		m_atkInfo.SetAttackBox(lt, rb);
	}
	else {
		POINT lt{ m_playerPt.x, m_playerPt.y - m_hitBoxSize.cy / 2 };
		POINT rb{ m_playerPt.x + 75, m_playerPt.y + m_hitBoxSize.cy / 2 };
		m_atkInfo.SetAttackBox(lt, rb);
	}

	m_atkInfo.attackDamage = 44;
	m_atkInfo.attackCountMax = 8;
	m_atkInfo.attackedEffectTag = "AttackedEffect001";
	m_atkInfo.knockBackPower = 0;
	m_atkInfo.effectSize = { 75, 75 };

	m_atkInfo.attackTime = (m_animation->GetClip("fastAtk")->GetLoopTime() * loopTime)
		/ m_atkInfo.attackCountMax;

	SetSpAnimation(true, loopTime, "fastAtk");
}

void Player::Attack() {
	if (m_spAnimation or m_spAnimationName == "hit") {
		return;
	}
	
	int loopTime = 1;

	int dir = m_lookLR == MD_LEFT ? -1 : 1;
	if (dir < 0) {
		POINT lt{ m_playerPt.x - 50, m_playerPt.y - m_hitBoxSize.cy / 2 };
		POINT rb{ m_playerPt.x, m_playerPt.y + m_hitBoxSize.cy / 2 };
		m_atkInfo.SetAttackBox(lt, rb);
	}
	else {
		POINT lt{ m_playerPt.x, m_playerPt.y - m_hitBoxSize.cy / 2 };
		POINT rb{ m_playerPt.x + 50, m_playerPt.y + m_hitBoxSize.cy / 2 };
		m_atkInfo.SetAttackBox(lt, rb);
	}
	m_atkInfo.attackDamage = 30;
	m_atkInfo.attackCountMax = 8;
	m_atkInfo.attackedEffectTag = "";
	m_atkInfo.attackCountMax = 1;
	m_atkInfo.knockBackPower = 100;

	m_atkInfo.attackTime = (m_animation->GetClip("fastAtk")->GetLoopTime() * loopTime)
		/ m_atkInfo.attackCountMax;

	SetSpAnimation(true, loopTime, "lightAtk");
}

void Player::Jump() {
	if (!m_jumped) {
		return;
	}

	m_jumped = true;
	m_power = static_cast<int>(m_jumpPowerOrigin);
	m_gravityTime = 0.f;
}

bool Player::Diying() const {
	return m_deadAnimation;
}

void Player::Die() {
	m_deadAnimation = true;
	m_spAnimation = true;
	m_noMoveAnimation = true;
	SetSpAnimation(true, 1, "die");
}

void Player::SetDefaultAnimation() {
	m_curClip->Reset();
	m_curClip = m_animation->GetClip("default");
	m_spAnimation = false;
	m_noMoveAnimation = false;
	m_spAnimationTime = 0.f;
	m_spAnimationLimitTime = 0.f;
	m_spAnimationLoop = 0;
	m_spAnimationLoopTime = 0;
	m_spAnimationName = "";
}

void Player::SetSpAnimation(bool noMove, int loopTime, const string& anName) {
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

bool Player::CheckSpAnimationEnd(float deltaTime) {
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

void Player::AnimationCancel() {
	if (m_skillEffect) {
		m_skillEffect->End();
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

void Player::SpAnimationEnd() {
	if (!m_spAnimation) {
		return;
	}

	if (m_deadAnimation) {
		m_alive = false;
		return;
	}
	else if (m_spAnimationName == "dash") {
	}

	if (m_skillEffect) {
		m_skillEffect->End();
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

void Player::Gravity(float deltaTime) {
	if (m_onGround and !m_jumped) {
		return;
	}
	
	m_gravityTime += deltaTime * m_timeScale;
	m_power -= static_cast<int>(gravity * m_gravityTime * m_gravityTime * m_timeScale);
	if (m_power <= m_limitPower) {
		m_power = static_cast<int>(m_limitPower);
	}

	m_prevPt = m_playerPt;
	int dist = static_cast<int>(floor(m_power * deltaTime * m_timeScale));
	
	m_playerPt.y -= dist;
	if (m_playerPt.y + m_hitBoxSize.cy / 2 > worldSize.cy) {
		SetPower(0);
		m_playerPt.y = worldSize.cy - m_hitBoxSize.cy / 2;
		if (m_jumped) {
			m_curClip = m_animation->GetClip("default");
		}
		m_jumped = false;
	}
}

void Player::UpdateEffectPt() {
	if (m_skillEffect) {
		int dir = m_lookLR == MD_LEFT ? -1 : 1;
		POINT effectPt{ m_playerPt.x + (dir * m_playerSize.cx / 2), m_playerPt.y };
		m_skillEffect->SetPt(effectPt);

		if (dir < 0) {
			POINT lt{ m_playerPt.x - 75, m_playerPt.y - m_hitBoxSize.cy / 2 };
			POINT rb{ m_playerPt.x, m_playerPt.y + m_hitBoxSize.cy / 2 };
			m_atkInfo.SetAttackBox(lt, rb);
		}
		else {
			POINT lt{ m_playerPt.x, m_playerPt.y - m_hitBoxSize.cy / 2 };
			POINT rb{ m_playerPt.x + 75, m_playerPt.y + m_hitBoxSize.cy / 2 };
			m_atkInfo.SetAttackBox(lt, rb);
		}
	}
}

bool Player::Save(FILE* fp) {
	fwrite(&m_playerPt, sizeof(POINT), 1, fp);
	fwrite(&m_playerSize, sizeof(SIZE), 1, fp);
	fwrite(&m_hitBoxSize, sizeof(SIZE), 1, fp);
	return true;
}

bool Player::Load(FILE* fp) {
	fread(&m_playerPt, sizeof(POINT), 1, fp);
	fread(&m_playerSize, sizeof(SIZE), 1, fp);
	fread(&m_hitBoxSize, sizeof(SIZE), 1, fp);
	return true;
}
