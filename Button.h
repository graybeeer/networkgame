#pragma once
#include "TextureManager.h"

#include "Animation.h"
#include "Player.h"
#include "Effect.h"
#include "Global.h"

class Button {
public:
	Button();
	Button(const POINT startPt);
	~Button();

private:
	Animation* m_animation{ };
	AnimationClip* m_curClip{ };

	Effect* m_effect{ };

	POINT m_prevPt{ };
	POINT m_buttonPt{ };
	SIZE m_buttonSize{ 60, 20 };

	int m_hp{ 1000 };
	float m_time{ };
	float m_reviveTime{ 10.f };

	// move
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

	bool m_invincible{ false };
	float m_invincibleTime{ 0.f };
	float m_invincibleEndTime{ 0.f };

	// jump and Gravity
	bool m_jumped{ false };
	const float m_limitPower{ -300.f };
	float m_power{ 0.f };
	float m_gravityTime{ 0.f };
	bool m_onGround{ false };

	// hitBox
	RECT m_hitBox{ };
	SIZE m_hitBoxSize{ 60, 20 };

	// alive
	bool m_alive{ true };

public:
	POINT GetButtonPt() const;
	SIZE GetButtonSize() const;
	RECT GetButtonRC() const;
	POINT GetPrevPt() const;

	RECT GetRenderRC() const;

	RECT GetPrevRC() const;

	void SetPt(const POINT& pt);

	void OnGround(int groundTop);
	void OnGround();

	bool IsInvincible() const;
	bool Diying() const;

	void Attacked(int attackDamage, int dir, float attackTime = -1.f, int knockBackPower = 0);
	void Invincible(float invincibleTime);
	void InvincibleEnd();

public:
	void Update(float deltaTime);
	void Render(HDC destDC);

	void SetDefaultAnimation();

	void SetSpAnimation(bool noMove, int loopTime, const string& anName);
	bool CheckSpAnimationEnd(float deltaTime);
	void SpAnimationEnd();

	void Die();
	void Revive();

	bool Save(FILE* fp);
	bool Load(FILE* fp);
};
