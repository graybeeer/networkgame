#pragma once

#include "framework.h"

class Timer {
private:
	Timer();
	~Timer();

private:
	static Timer* m_pInstance;

public:
	static Timer* GetInst();
	static void Destroy();

private:
	LARGE_INTEGER m_tick;
	LARGE_INTEGER m_prev;
	float m_timeScale;
	float m_deltaTime;

public:
	void SetTimeScale(float timeScale);
	float GetDeltaTime() const;
	float GetTimeScale() const;

public:
	bool Init();
	void Update();

};

