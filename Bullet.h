#pragma once

#include "TextureManager.h"

#include "Animation.h"
#include "AnimationClip.h"

class Bullet {
public:
	Bullet() = delete;
	Bullet(const string& tag, const POINT& pt, const SIZE& size, int moveDir, float moveAngle = 0);
	Bullet(const string& tag, const POINT& pt, int moveDir, float moveAngle = 0);
	~Bullet();

	friend struct BulletManager;

private:
	Animation* m_animation{ };
	AnimationClip* m_curClip{ };
	POINT m_bulletPt{ };
	SIZE m_bulletSize{ 150, 150 };

	string m_strTag{ };

	int m_moveDist{ };
	int m_limitMoveDist{ 500 };

	float m_angle{ };
	float m_speed{ 700.f };
	int m_moveDir{ };
	float m_time{ };
	float m_limitTime{ 7.f };

	RECT m_hitBox{ };
	SIZE m_hitBoxSize{ 80, 50 };

	bool m_moveAngle{ false };
	bool m_targeting{ false };
	bool m_alive{ true };

public:
	RECT GetRenderRC() const;
	POINT GetBulletPt() const;
	SIZE GetBulletSize() const;

	void SetBulletPt(const POINT& pt);

	bool GetTargetingMode() const;
	const string& GetTag() const;

	RECT GetBulletRC() const;
	bool IsAlive() const;
	float GetAngle() const;
	
	void CheckOut();

public:
	void Update(float deltaTime, const POINT& targetPt);
	void Update(float deltaTime);
	void Render(HDC destDC);

	void Move(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);

	void MoveToTarget(float deltaTime, const POINT& targetPt);
	void MoveAngle(float deltaTime, int angle);
	void MoveAngle(float deltaTime, float angle);

	void Die();
	bool Save(FILE* fp);
	bool Load(FILE* fp);
};