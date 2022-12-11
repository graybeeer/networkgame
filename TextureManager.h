#pragma once

#include "framework.h"


class TextureManager {
private:
	TextureManager();
	~TextureManager();

private:
	static TextureManager* m_pInstance;

public:
	static TextureManager* GetInst();
	static void Destroy();

private:
	HINSTANCE m_hInst{ };
	HDC m_hMainDC{ };
	vector<pair<string, wstring>> m_textureInfo{ };
	map<string, CImage*> m_textureMap{ };

public:

	bool Init(HINSTANCE hInst, HDC mainDC, const wchar_t* backGroundFileName);
	void Update();

	bool FindTexture(const string& strKey);

	bool AddNewTexture(const string& strKey, const wstring& fileName);
	bool AddNewTextrue(const string& strKey, const wchar_t* fileName);

	CImage* GetTexture(const string& strKey);

	bool Save();
	bool Load();

};

