#pragma once

#include "framework.h"
#include "AnimationManager.h"
#include "Structures.h"
#include "HPBar.h"

enum class BossState {
	ST_DEFAULT,
	ST_MOVE,
	ST_PATTERN,
	ST_DOWN,
	ST_DIE,
	ST_CHARGING,
	ST_LAZER,
	ST_ATTACK,
	ST_FIREREADY,
};

struct BossAI {

public:
	BossState m_curState{ BossState::ST_DEFAULT};
	float m_stateTime{ };
	float m_stateEndTime{ };
	int m_patternNum{ };

public:
	BossState GetState() {
		m_stateEndTime = GetRandReal(0.5, 3.0);

		switch (GetRandInt(0, 4)) {
		case 0:
			return BossState::ST_DEFAULT;

		case 1:
		case 2:
		case 3:
		case 4:
			m_patternNum = GetRandInt(0, 1);
			return BossState::ST_PATTERN;

		}
	}

	void StateEnd() {
		m_stateTime = 0.f;
		m_curState = GetState();
	}

	void Update(float deltaTime) {
		m_stateTime += deltaTime;
		if (m_stateTime >= m_stateEndTime) {
			StateEnd();
		}
	}
};

class Boss {
public:
	Boss() = delete;
	Boss(const POINT& pt);
	~Boss();

private:
	POINT m_playerPt{ };

private:
	class Effect* m_moveEffect{ };

	Animation* m_animation{ };
	AnimationClip* m_curClip{ };

	POINT m_pt{ };
	POINT m_prevPt{ };
	SIZE m_size{ 800, 800 };

	int m_hp{ 1000 };

	BossAI m_ai{ };
	BossState m_state{ BossState::ST_DEFAULT };
	
	bool m_warning{ false };
	bool m_targeting{ true };
	bool m_stPattern{ false };
	bool m_patternReadyEnd{ false };

	float m_fireTime{ };
	const float m_fireLimitTime{ 1.25f };

	float m_angle{ 0.f };

	// jump
	int m_power{ 0 };
	int m_limitPower{ -200 };
	float m_timeScale{ 1.5f };
	float m_gravityTime{ 0.f };
	bool m_onGround{ false };
	bool m_flying{ true };

	Effect* m_skillEffect{ };
	Effect* m_barriorEffect{ };

	// animation
	float m_spAnimationTime{ 0.f };
	float m_spAnimationLimitTime{ 0.f };
	int m_spAnimationLoop{ 0 };
	int m_spAnimationLoopTime{ 0 };
	bool m_noMoveAnimation{ false };
	bool m_spAnimation{ false };
	string m_spAnimationName{ };
	bool m_deadAnimation{ false };

	// effect
	float m_effectTime{ };
	float m_effectLimitTime{ };
	int m_effectLoop{ };
	int m_effectLoopTime{ };

	// move
	float m_speedOrigin{ 300.f };
	float m_speed{ m_speedOrigin };
	int m_look{ MD_NULL };
	int m_lookLR{ MD_RIGHT };
	bool m_dash{ false };

	// attacked
	bool m_alive{ true };
	SIZE m_hitBoxSizeOrigin{ 48, 46 };
	SIZE m_hitBoxSize{ m_hitBoxSizeOrigin };

	bool m_knockBack{ false };
	int m_knockBackPower{ -1 };
	const float m_knockBackScale{ 1.5f };
	int m_knockBackDist{ };
	int m_knockBackDir{ };

	bool m_invincible{ false };
	float m_invincibleTime{ 0.f };
	float m_invincibleEndTime{ 0.f };

	// attack
	ATTACKINFO m_atkInfo{ };

	HPBAR m_hpBar{ m_pt, m_hitBoxSize, m_hp, 0x0000ff };

public:
	RECT GetRC() const;
	RECT GetRenderRC() const;
	POINT GetPt() const;
	SIZE GetSize() const;

	void ResetJumpPower();
	void SetPower(int power, bool timeClear = false);
	void OnGround(int groundTop);

	void HDdown(int downHP);

	bool IsInvincible() const;

	void Invincible(float invincibleTime);
	void InvincibleEnd();

	bool IsAlive() const;

private:
	// Animation
	void SetDefaultAnimation();
	void SetSpAnimation(bool noMove, int loopTime, const string& anName);
	bool CheckSpAnimationEnd(float deltaTime);
	void AnimationCancel();
	void SpAnimationEnd();

	void Gravity(float deltaTime);

	void UpdateEffectPt();

	void AttackGround(float deltaTime);
	void FireBullet(float deltaTime);
	void AttackLazer(float deltaTime);

	void Die();

public:
	void Update(float deltaTime, const POINT& playerPt);
	void Render(HDC destDC);

};