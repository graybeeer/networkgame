#pragma once

#include "framework.h"
#include "Effect.h"

class EffectManager {
	EffectManager();
	~EffectManager();

private:
	static EffectManager* m_pInst;

public:
	static EffectManager* GetInst();
	static void Destroy();

private:
	unordered_map<string, Effect*> m_protoEffectMap;
	list<Effect*> m_effectInWorldList;

public:
	RECT GetEffectImgRC(const string& effectKey) const;

public:
	void Reset();

	void AddNewEffect(const string& effectKey, const wstring& fileName,
		const string& clipKey, int frameLength, float frameTime);

	void LoadFromeFile(const string& fileName);

	Effect* CreateEffectInWorld(const string& effectKey, const SIZE& imgSize, const POINT& pt, int loopTime);

	void Update(float deltaTime);
	void DeleteEndEffect();
	void Render(HDC destDC);
	void Render(HDC destDC, bool rev);
};

