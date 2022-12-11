#pragma once

#include "framework.h"
#include "AnimationManager.h"
#include "Global.h"

enum class state {
	ST_DEFAULT,
	ST_MOVE,
	ST_JUMP,
	ST_TRACE,
	ST_ATTACK
};

class MonsterAI {
public:
	MonsterAI() = delete;
	MonsterAI(int maxState) : m_maxState{ maxState } {
		if (m_maxState < 1) {
			m_maxState = 1;
		}
	}
	~MonsterAI() { }

private:
	int m_maxState;
	state m_state{ state::ST_DEFAULT };
	float m_stateTime{ };
	float m_stateEndTime{ 3.f };

public:
	state Update(float deltaTime, int& moveDir) {
		if (m_stateTime >= m_stateEndTime) {
			m_stateEndTime = (float)GetRandReal(1.0, 3.0);
			switch (GetRandInt(0, m_maxState)) {
			case 0:
				m_state = state::ST_DEFAULT;
				break;

			case 1:
				m_state = state::ST_MOVE;
				moveDir = GetRandInt(0, 1) == 0 ? MD_LEFT : MD_RIGHT;
				break;

			case 2:
				m_state = state::ST_JUMP;
				break;

			case 3:
				m_state = state::ST_TRACE;
				break;

			case 4:
				m_state = state::ST_ATTACK;
				break;
			}

			m_stateTime = 0.f;
		}

		m_stateTime += deltaTime;

		return m_state;
	}

	state Update(float deltaTime) {
		if (m_stateTime >= m_stateEndTime) {
			m_stateEndTime = (float)GetRandReal(1.0, 3.0);
			switch (GetRandInt(0, m_maxState)) {
			case 0:
				m_state = state::ST_DEFAULT;
				break;

			case 1:
				m_state = state::ST_MOVE;
				break;

			case 2:
				m_state = state::ST_JUMP;
				break;

			case 3:
				m_state = state::ST_TRACE;
				break;

			case 4:
				m_state = state::ST_ATTACK;
				break;
			}

			m_stateTime = 0.f;
		}

		m_stateTime += deltaTime;

		return m_state;
	}

	bool OnGround(const RECT& groundRect, const RECT& monsterRC, int nMoveDist) {
		if (nMoveDist > 0) {
			if (monsterRC.right + nMoveDist >= groundRect.right) {
				return false;
			}
		}
		else {
			if (monsterRC.left + nMoveDist <= groundRect.left) {
				return false;
			}
		}

		return true;
	}
};

struct PIXEL {
	unsigned char r{ };
	unsigned char g{ };
	unsigned char b{ };

	bool operator==(const PIXEL other) {
		if (r == other.r and g == other.g and b == other.b) {
			return true;
		}

		return false;
	} 

	bool operator==(const COLORREF other) {
		if (r == (other & 0xff) and g == ((other >> 8) & 0xff) and b == ((other >> 16) & 0xff)) {
			return true;
		}

		return false;
	}

	const PIXEL& operator=(const PIXEL other) {
		r = other.r;
		g = other.g;
		b = other.b;

		return *this;
	}

	const PIXEL& operator=(const COLORREF other) {
		r = other & 0xff;
		g = (other >> 8) & 0xff;
		b = (other >> 16) & 0xff;

		return *this;
	}
};

struct ATTACKINFO {
	SIZE attackImgSize{ };
	SIZE attackBoxSize{ };
	POINT attackStPt{ };
	RECT attackBox{ };

	string attackedEffectTag{ };
	SIZE effectSize{ };

	int attackDamage{ };
	float attackTime{ };
	int knockBackPower{ };

	int attackCountMax{ };
	int attackObject{ };
	int attackObjectMax{ };

	const ATTACKINFO& GetInfo() const {
		return *this;
	}

	void SetAttackBox(const POINT& lt, const POINT& rb) {
		attackBox = { lt.x, lt.y, rb.x, rb.y };
	}

	void SetAttackBox(const SIZE& sz, const POINT& center) {
		attackBox = { center.x - sz.cx / 2, center.y - sz.cy / 2,
			center.x + sz.cx, center.y + sz.cy };
	}

	void Reset() {
		attackImgSize = { };
		attackBoxSize = { };
		attackStPt = { };
		attackBox = { };

		attackedEffectTag = { };
		effectSize = { };

		attackDamage = { };
		attackTime = { };
		knockBackPower = { };

		attackTime = { };
		attackCountMax = { };
		attackObject = { };
		attackObjectMax = { };
	}
};

struct ANIMATIONCLIPINFO {
	string strKey{ };
	POINT startPt{ };
	int frameLength{ };
	float frameTime{ };
};

struct EFFECTINFO {
	string effectAnName{ };
	string effectClipName{ };
	string effectName{ };
	SIZE effectImgSize{ };
	POINT effectPt{ };

	EFFECTINFO(const string& anName, const string& clipName, const string& efName) {

	}

	~EFFECTINFO() {

	}

	void Update(float deltaTime) {

	}
	
	void Render(float deltaTime) {

	}
};

struct BUFFINFO {
	int hpHeal{ };
	int hpUp{ };
	float hpUpPercent{ };
	int mpHeal{ };
	int mpUp{ };
	float mpUpPercent{ };

	int jumpPowerUp{ };
	float jumpPowerUpPercent{ };

	int damageUp{ };
	float damageUpPercent{ };
};