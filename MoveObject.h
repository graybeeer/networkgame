#pragma once
#include "Object.h"
class MoveObject :
    public Object {
public:
    MoveObject(const string& animationKey);
    virtual ~MoveObject();

protected:
    Animation* m_animation{ };
    AnimationClip* m_curClip{ };

	// move
	float m_speed{ 300.f };
	int m_look{ MD_NULL };
	int m_lookLR{ MD_RIGHT };

	// jump and gravity
	bool m_jumped{ false };
	bool m_djump{ false };
	const float m_jumpPowerOrigin{ 200.f };
	const float m_limitPower{ -500.f };
	int m_power{ 0 };
	float m_timeScale{ 1.5f };
	float m_gravityTime{ 0.f };
	bool m_onGround{ false };

	// attacked
	bool m_alive{ true };

public:
	virtual void Update(float deltaTime) override;
	virtual void Render(HDC destDC) override;
	virtual void Collision(const string& strTag) override;
	virtual void Input(float deltaTime);

	void Move(float deltaTime, int dir);
	void MoveLeft(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);

	void MoveDist(float deltaTime, int dir, int dist);

	void Gravity(float deltaTime);
};

