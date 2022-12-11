#pragma once

#include "framework.h"
#include "Animation.h"
#include "Bullet.h"
#include "hpBar.h"
#include "Effect.h"

class Monster {
public:
	Monster();
	Monster(const POINT startPt);
	Monster(const POINT startPt, int moveDir);
	virtual ~Monster();

protected:
	Animation* m_animation{ };
	AnimationClip* m_curClip{ };

	MonsterAI m_ai{ 1 };

	Effect* m_effect{ };

	POINT m_prevPt{ };
	POINT m_monsterPt{ };
	SIZE m_monsterSize{ 50, 50 };

	int m_hp{ 1000 };

	string m_tag{ };
	COLORREF m_colorKey{ };

	// move
	float m_speed{ 200.f };
	int m_moveDir{ MD_RIGHT };
	int m_look{ MD_RIGHT };
	int m_lookLR{ MD_RIGHT };

	// animation
	float m_spAnimationTime{ 0.f };
	float m_spAnimationLimitTime{ 0.f };
	int m_spAnimationLoop{ 0 };
	int m_spAnimationLoopTime{ 0 };

	bool m_deadAimation{ false };

	bool m_spAnimation{ false };
	bool m_noMoveAnimation{ false };
	string m_spAnimationName{ };

	// attacked or attack
	bool m_knockBack{ false };
	int m_knockBackPower{ };
	const float m_knockBackScale{ 1.5f };
	int m_knockBackDist{ };
	int m_knockBackDir{ };

	int m_damage{ };

	bool m_invincible{ false };
	float m_invincibleTime{ 0.f };
	float m_invincibleEndTime{ 0.f };

	// jump and Gravity
	bool m_jumped{ false };
	const float m_limitPower{ -300.f };
	float m_power{ 0.f };
	float m_gravityTime{ 0.f };
	bool m_onGround{ false };

	RECT m_groundRC{ };

	// hitBox
	RECT m_hitBox{ };
	SIZE m_hitBoxSize{ 40, 40 };

	// alive
	bool m_alive{ true };

	HPBAR m_hpBar{ m_monsterPt, m_hitBoxSize, m_hp, RGB(255, 255, 0) };

protected:
	void SetColorKey(const COLORREF& colorKey);

public:
	POINT GetMonsterPt() const;
	SIZE GetMonsterSize() const;
	RECT GetMonsterRC() const;
	POINT GetPrevPt() const;

	RECT GetRenderRC() const;

	RECT GetPrevRC() const;

	const string& GetTag() const;

	void SetPt(const POINT& pt);

	void OnGround(int groundTop, const RECT& groundRC);
	void OnGround();

	bool IsAlive() const;
	bool IsInvincible() const;
	bool Diying() const;

	void Gravity(float deltaTime);

	void Attacked(int attackDamage, int dir, float attackTime = -1.f, int knockBackPower = 0);
	void Attacked(ATTACKINFO& atkInfo, int dir);
	void Invincible(float invincibleTime);
	void InvincibleEnd();

public:
	void Update(float deltaTime);
	void Render(HDC destDC);

	void Input(float deltaTime);

	void MoveDist(float deltaTime, int dist, int moveDir);

	int Move(float deltaTime);
	int MoveLeft(float deltaTime);
	int MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);

	void SetDefaultAnimation();

	void SetSpAnimation(bool noMove, int loopTime, const string& anName);
	bool CheckSpAnimationEnd(float deltaTime);
	void SpAnimationEnd();

	void Die();

	bool Save(FILE* fp);
	bool Load(FILE* fp);
};
