#pragma once

#include "TextureManager.h"

#include "Animation.h"
#include "AnimationClip.h"
#include "Bullet.h"
#include "Global.h"
#include "Hpbar.h"
#include "Effect.h"

class Player {
public:
	Player(bool myControl, int playerNum);
	Player(bool myControl, int playerNum, const POINT& pt);
	~Player();

private:
	// default Info
	Animation* m_animation{ };
	AnimationClip* m_curClip{ };

	POINT m_playerPt{ 100, worldSize.cy - 200 };
	POINT m_prevPt{ };
	SIZE m_playerSize{ 100, 100 };

	int m_hp{ 3000 };
	bool m_myControl{ false }; //이 캐릭터가 내가 조정하는 캐릭터인지

	// jump
	bool m_jumped{ false };
	bool m_djump{ false };
	const float m_jumpPowerOrigin{ 200.f };
	const float m_limitPower{ -500.f };
	int m_power{ 0 };
	float m_timeScale{ 1.5f };
	float m_gravityTime{ 0.f };
	bool m_onGround{ false };

	Effect* m_skillEffect{ };

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
	Animation* m_effectAnimation{ };
	AnimationClip* m_effectClip{ };
	float m_effectTime{ };
	float m_effectLimitTime{ };
	int m_effectLoop{ };
	int m_effectLoopTime{ };

	// move
	const float m_speedOrigin{ 300.f };
	float m_speed{ m_speedOrigin };
	int m_look{ MD_NULL };
	int m_lookLR{ MD_RIGHT };
	bool m_dash{ false };

	// attacked
	bool m_alive{ true };
	SIZE m_hitBoxSizeOrigin{ 20, 38 };
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

	HPBAR m_hpBar{ m_playerPt, m_hitBoxSize, m_hp, 0x0000ff };

public:
	POINT GetPlayerPt() const;
	SIZE GetPlayerSize() const;
	RECT GetPlayerRC() const;
	POINT GetPrevPt() const;
	SIZE GetHitBoxSize() const;

	RECT GetRenderRC() const;

	int GetKnockBackPower() const;

	ATTACKINFO GetAttackInfo() const;

	int GetHP() const;

	RECT GetPrevRC() const;

	void ResetJumpPower();
	void SetPower(int power, bool timeClear = false);
	void OnGround(int groundTop);
	void OnGround();

	void SetX(int x);
	void SetPt(const POINT& pt);
	void HDdown(int downHP);

	bool IsInvincible() const;
	bool IsAlive() const;

	void Invincible(float invincibleTime);
	void InvincibleEnd();

private:
	// Animation
	void SetDefaultAnimation();
	void SetSpAnimation(bool noMove, int loopTime, const string& anName);
	bool CheckSpAnimationEnd(float deltaTime);
	void AnimationCancel();
	void SpAnimationEnd();

	void Gravity(float deltaTime);

	void UpdateEffectPt();

public:
	void Update(float deltaTime);
	void Render(HDC destDC);

	void Input(float deltaTime);

	void MoveDist(float deltaTime, int dist, int moveDir);

	void Move(float deltaTime, int moveDir);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);

	void CheckOut();

	void Attacked(int attackDamage, int dir, int knockBackPower = 0);
	void Dash();

	// 
	void Skill1();
	void FastAttack();
	void Attack();
	void Jump();
	bool Diying() const;
	void Die();

	bool Save(FILE* fp);
	bool Load(FILE* fp);
};

