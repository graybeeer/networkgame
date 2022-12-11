#include "EffectManager.h"
#include "Global.h"

EffectManager* EffectManager::m_pInst = nullptr;

EffectManager::EffectManager() { }

EffectManager::~EffectManager() {
    auto lIterEnd = m_effectInWorldList.end();
    for (auto iter = m_effectInWorldList.begin(); iter != lIterEnd; ++iter) {
        if (*iter) {
            delete (*iter);
        }
        (*iter) = nullptr;
    }
    m_effectInWorldList.clear();

    auto mIterEnd = m_protoEffectMap.end();
    for (auto iter = m_protoEffectMap.begin(); iter != mIterEnd; ++iter) {
        if ((iter->second)) {
            delete iter->second;
        }
        iter->second = nullptr;
    }
    m_protoEffectMap.clear();
}

EffectManager* EffectManager::GetInst() {
    if (!m_pInst) {
        m_pInst = new EffectManager;
    }
    return m_pInst;
}

void EffectManager::Destroy() {
    if (m_pInst) {
        delete m_pInst;
    }
    m_pInst = nullptr;
}

RECT EffectManager::GetEffectImgRC(const string& effectKey) const {
    auto iter = m_protoEffectMap.find(effectKey);
    if (iter == m_protoEffectMap.end()) {
        return RECT{ -1, -1 };
    }

    return iter->second->GetRC();
}

void EffectManager::Reset() {
    auto iterEnd = m_effectInWorldList.end();
    for (auto iter = m_effectInWorldList.begin(); iter != iterEnd; ++iter) {
        (*iter)->End();
    }

    DeleteEndEffect();
}

void EffectManager::AddNewEffect(const string& effectKey, const wstring& fileName,
    const string& clipKey, int frameLength, float frameTime) {
}

void EffectManager::LoadFromeFile(const string& fileName) {
    FILE* fp{ };
    fopen_s(&fp, fileName.c_str(), "r");

    while (feof(fp) == 0) {
        string key{ };
        FreadString(key, fp);

        string file{ };
        FreadString(file, fp);
        wstring fileW{ };
        fileW.assign(file.begin(), file.end());

        POINT maxFrame{ };
        int frameLength{ };
        float frameTime{ };
        COLORREF colorKey{ };
        int alphaBlend{ 0xff };
        if (EOF == fscanf(fp, "%d %d %d %f %d %d\n",
            &maxFrame.x, &maxFrame.y, &frameLength, &frameTime, &colorKey, &alphaBlend)) {
            break;
        }

        AnimationManager::GetInst()->AddNewAnimation(key, fileW, maxFrame);

        Effect* newEffect = new Effect{ key, frameLength, frameTime, colorKey, alphaBlend };
        m_protoEffectMap.insert(make_pair(key, newEffect));
    }
}

Effect* EffectManager::CreateEffectInWorld(const string& effectKey, const SIZE& imgSize, const POINT& pt, int loopTime) {
    auto iter = m_protoEffectMap.find(effectKey);
    if (iter == m_protoEffectMap.end()) {
        return nullptr;
    }

    Effect* newEffect = new Effect{ *(iter->second) };
    newEffect->SetEffectInfo(pt, imgSize, imgSize, loopTime, false);
    m_effectInWorldList.push_back(newEffect);

    return newEffect;
}

void EffectManager::Update(float deltaTime) {
    auto iterEnd = m_effectInWorldList.end();
    for (auto iter = m_effectInWorldList.begin(); iter != iterEnd; ++iter) {
        (*iter)->Update(deltaTime);
    }
}

void EffectManager::DeleteEndEffect() {
    auto iterEnd = m_effectInWorldList.end();
    for (auto iter = m_effectInWorldList.begin(); iter != iterEnd;) {
        if ((*iter)->EffectEnd()) {
            (*iter)->Reset();
            Effect* delTarget = (*iter);
            iter = m_effectInWorldList.erase(iter);
            delete delTarget;
            iterEnd = m_effectInWorldList.end();
        }
        else {
            ++iter;
        }
    }
}

void EffectManager::Render(HDC destDC) {
    auto iterEnd = m_effectInWorldList.end();
    for (auto iter = m_effectInWorldList.begin(); iter != iterEnd; ++iter) {
        if ((*iter)->RotatePrint()) {
            (*iter)->RenderRotate(destDC);
        }
        else {
            if ((*iter)->RevPrint()) {
                (*iter)->Render(destDC, true);
            }
            else {
                (*iter)->Render(destDC);
            }
        }
    }
}

void EffectManager::Render(HDC destDC, bool rev) {
    auto iterEnd = m_effectInWorldList.end();
    for (auto iter = m_effectInWorldList.begin(); iter != iterEnd; ++iter) {
        if ((*iter)->RotatePrint()) {
            (*iter)->RenderRotate(destDC);
        }
        else {
			if ((*iter)->RevPrint()) {
				(*iter)->Render(destDC, true);
			}
			else {
				(*iter)->Render(destDC);
			}
        }
    }
}
