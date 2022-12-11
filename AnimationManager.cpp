#include "AnimationManager.h"
#include "TextureManager.h"
#include "Global.h"

AnimationManager* AnimationManager::m_pInst = nullptr;

AnimationManager::AnimationManager() { }

AnimationManager::~AnimationManager() {
	map<string, Animation*>::iterator iter = m_animationMap.begin();
	map<string, Animation*>::iterator iterEnd = m_animationMap.end();

	for (; iter != iterEnd; ++iter) {
		if (iter->second) {
			delete iter->second;
		}
		iter->second = nullptr;
	}

	m_animationMap.clear();
}

AnimationManager* AnimationManager::GetInst() {
	if (!m_pInst) {
		m_pInst = new AnimationManager;
	}

	return m_pInst;
}

void AnimationManager::Destroy() {
	if (m_pInst) {
		delete m_pInst;
	}

	m_pInst = nullptr;
}

bool AnimationManager::Init(HINSTANCE hInst) {
	m_hInst = hInst;

	return true;
}

bool AnimationManager::AddNewAnimation(const string& strKey, const wstring& fileName, POINT maxFrameLength) {
	TextureManager::GetInst()->AddNewTextrue(strKey, fileName.c_str());
	CImage* srcImg = TextureManager::GetInst()->GetTexture(strKey);

	map<string, Animation*>::iterator iter = m_animationMap.find(strKey);
	if (iter == m_animationMap.end()) {
		Animation* newAnimation = new Animation;
		if (!newAnimation->LoadSrcImg(srcImg, fileName, maxFrameLength)) {
			delete newAnimation;
			return false;
		}
		m_animationMap.insert(make_pair(strKey, newAnimation));
		m_keyInfo.push_back(make_pair(strKey, fileName));
	}

	return true;
}

bool AnimationManager::AddNewAnimation(const string& strKey, const wstring& fileName, POINT maxFrameLength,
	const vector<string>& keyVec, const vector<POINT>& stPts, const vector<int>& lengths, const vector<float>& times) {

	TextureManager::GetInst()->AddNewTextrue(strKey, fileName.c_str());
	CImage* srcImg = TextureManager::GetInst()->GetTexture(strKey);

	map<string, Animation*>::iterator iter = m_animationMap.find(strKey);
	if (iter == m_animationMap.end()) {
		Animation* newAnimation = new Animation;
		if (!newAnimation->LoadSrcImg(srcImg, fileName, maxFrameLength)) {
			delete newAnimation;
			return false;
		}

		if (!SetClips(strKey, keyVec, stPts, lengths, times)) {
			return false;
		}

		m_animationMap.insert(make_pair(strKey, newAnimation));
		m_keyInfo.push_back(make_pair(strKey, fileName));
	}

	return true;
}

bool AnimationManager::AddNewAnimation(const string& strKey, const wstring& fileName, POINT maxFrameLength,
	const string* keyArr, const POINT* stPtArr, int* lengthArr, float* timeArr, int clipSize) {

	TextureManager::GetInst()->AddNewTextrue(strKey, fileName.c_str());
	CImage* srcImg = TextureManager::GetInst()->GetTexture(strKey);
	
	map<string, Animation*>::iterator iter = m_animationMap.find(strKey);
	if (iter == m_animationMap.end()) {
		Animation* newAnimation = new Animation;
		if (!newAnimation->LoadSrcImg(srcImg, fileName, maxFrameLength)) {
			delete newAnimation;
			return false;
		}

		if (!SetClips(strKey, keyArr, stPtArr, lengthArr, timeArr, clipSize)) {
			return false;
		}

		m_animationMap.insert(make_pair(strKey, newAnimation));
		m_keyInfo.push_back(make_pair(strKey, fileName));
	}

	return true;
}

bool AnimationManager::SetClips(const string& srcKey, const string* keyArr,
	const POINT* stPtArr, int* lengthArr, float* timeArr, int clipSize) {
	auto iter = m_animationMap.find(srcKey);
	if (iter == m_animationMap.end()) {
		return false;
	}

	Animation* dest = iter->second;
	for (int i = 0; i < clipSize; ++i) {
		if (!dest->AddNewClip(keyArr[i], stPtArr[i], lengthArr[i], timeArr[i])) {
			return false;
		}
	}

	return true;
}

bool AnimationManager::SetClips(const string& strKey, const vector<string>& keyVec, const vector<POINT>& stPts,
	const vector<int>& lengths, const vector<float>& times) {
	auto iter = m_animationMap.find(strKey);
	if (iter == m_animationMap.end()) {
		return false;
	}

	unsigned idxSize = GetMax(keyVec.size(), stPts.size(), lengths.size(), times.size(), unsigned{ });

	Animation* dest = iter->second;
	for (unsigned i = 0; i < idxSize; ++i) {
		if (!dest->AddNewClip(keyVec[i], stPts[i], lengths[i], times[i])) {
			return false;
		}
	}

	return true;
}

Animation* AnimationManager::GetAnimation(const Animation* other) const {
	if (!other) {
		return nullptr;
	}

	wstring othersFile = other->GetSrcFileName();
	auto iterEnd = m_keyInfo.end();
	for (auto iter = m_keyInfo.begin(); iter != iterEnd; ++iter) {
		if ((*iter).second == othersFile) {
			return GetAnimation((*iter).first);
		}
	}

	return nullptr;
}

Animation* AnimationManager::GetAnimation(const string& strKey) const {
	map<string, Animation*>::const_iterator iter = m_animationMap.find(strKey);

	if (iter == m_animationMap.end()) {
		return nullptr;
	}

	const Animation* protoAnimation = (*iter).second;

	TextureManager::GetInst()->AddNewTextrue(strKey, protoAnimation->GetSrcFileName().c_str());

	CImage* srcImg = TextureManager::GetInst()->GetTexture(strKey);

	Animation* cloneAnimation = new Animation(*protoAnimation);

	return cloneAnimation;
}
