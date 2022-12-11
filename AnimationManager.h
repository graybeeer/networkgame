#pragma once

#include "Animation.h"
#include "TextureManager.h"
#include "framework.h"

class AnimationManager {
private:
	AnimationManager();
	~AnimationManager();

private:
	static AnimationManager* m_pInst;

public:
	static AnimationManager* GetInst();
	static void Destroy();

private:
	HINSTANCE m_hInst{ };
	map<string, Animation*> m_animationMap;
	list<pair<string, wstring>> m_keyInfo;

public:
	bool Init(HINSTANCE hInst);
	bool AddNewAnimation(const string& strKey, const wstring& fileName, POINT maxFrameLength);

	bool AddNewAnimation(const string& strKey, const wstring& fileName, POINT maxFrameLength,
		const vector<string>& keyVec, const vector<POINT>& stPts, const vector<int>& lengths, const vector<float>& times);

	bool AddNewAnimation(const string& strKey, const wstring& fileName, POINT maxFrameLength,
		const string* keyArr, const POINT* stPtArr, int* lengthArr, float* timeArr, int clipSize);

	bool SetClips(const string& srcKey, const string* keyArr, const POINT* stPtArr,
		int* lengthArr, float* timeArr, int clipSize);

	bool SetClips(const string& strKey, const vector<string>& keyVec, const vector<POINT>& stPts,
		const vector<int>& lengths, const vector<float>& times);

	Animation* GetAnimation(const Animation* other) const;

	Animation* GetAnimation(const string& strKey) const;

	bool Load();

};

