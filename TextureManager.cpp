#include "TextureManager.h"
#include "Global.h"

bool once = false;

TextureManager* TextureManager::m_pInstance = nullptr;

TextureManager::TextureManager() : m_hInst{ }, m_textureMap{ } { }

TextureManager::~TextureManager() {
	map<string, CImage*>::iterator iter = m_textureMap.begin();
	map<string, CImage*>::iterator iterEnd = m_textureMap.end();
	for (; iter != iterEnd; ++iter) {
		if (iter->second) {
			delete iter->second;
		}
		iter->second = nullptr;
	}

	m_textureMap.clear();
}

TextureManager* TextureManager::GetInst() {
	if (!m_pInstance) {
		m_pInstance = new TextureManager;
	}

	return m_pInstance;
}

void TextureManager::Destroy() {
	if (m_pInstance) {
		delete m_pInstance;
	}

	m_pInstance = nullptr;

}

bool TextureManager::Init(HINSTANCE hInst, HDC mainDC, const wchar_t* backGroundFileName) {
	m_hInst = hInst;
	if (!m_hInst) {
		return false;
	}

	m_hMainDC = mainDC;

	return true;
}

void TextureManager::Update() {
}

bool TextureManager::FindTexture(const string& strKey) {
	map<string, CImage*>::iterator iter = m_textureMap.find(strKey);
	if (iter == m_textureMap.end()) {
		return false;
	}
	return true;
}

bool TextureManager::AddNewTexture(const string& strKey, const wstring& fileName) {
	map<string, CImage*>::iterator iter = m_textureMap.find(strKey);
	if (iter == m_textureMap.end()) {
		CImage* newTexture = new CImage;
		if (FAILED(newTexture->Load(fileName.c_str()))) {
			return false;
		}

		m_textureMap.insert(make_pair(strKey, newTexture));
		m_textureInfo.push_back(make_pair(strKey, fileName));
	}

	return true;
}

bool TextureManager::AddNewTextrue(const string& strKey, const wchar_t* fileName) {
	map<string, CImage*>::iterator iter = m_textureMap.find(strKey);
	if (iter == m_textureMap.end()) {
		CImage* newTexture = new CImage;
		if (FAILED(newTexture->Load(fileName))) {
			return false;
		}

		m_textureMap.insert(make_pair(strKey, newTexture));
		m_textureInfo.push_back(make_pair(strKey, wstring{ fileName }));
	}

	return true;
}

CImage* TextureManager::GetTexture(const string& strKey) {
	map<string, CImage*>::iterator iter = m_textureMap.find(strKey);
	map<string, CImage*>::iterator iterEnd = m_textureMap.end();

	if (iter == iterEnd) {
		return nullptr;
	}

	return (*iter).second;
}

bool TextureManager::Save() {
	SetCurrentDirectory(workingDirNameW.c_str());
	FILE* fp{ };
	fopen_s(&fp, tSaveFile.c_str(), "wb");
	if (!fp) {
		return false;
	}

	vector<pair<string, wstring>>::iterator iterEnd = m_textureInfo.end();
	//map<string, CImage*>::iterator iterEnd = m_textureMap.end();
	for (auto iter = m_textureInfo.begin(); iter != iterEnd; ++iter) {
		map<string, CImage*>::iterator tIter = m_textureMap.find((*iter).first);
		if (tIter != m_textureMap.end()) {
			FwriteString((*iter).first, fp);
			FwriteString((*iter).second, fp);
		}
	}

	fclose(fp);

	return true;
}

bool TextureManager::Load() {
	if (!FindFile(tSaveFile.c_str())) {
		return false;
	}

	SetCurrentDirectory(workingDirNameW.c_str());

	FILE* fp{ };
	fopen_s(&fp, tSaveFile.c_str(), "rb");
	if (!fp) {
		return false;
	}


	while (!feof(fp)) {
		string key{ };
		wstring file{ };
		FreadString(key, fp);
		FreadString(file, fp);
		AddNewTextrue(key, file.c_str());
	}

	fclose(fp);

	return true;
}