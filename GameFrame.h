#pragma once

#include "framework.h"
#include "Global.h"
#include "TextureManager.h"
#include "AnimationManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Monster.h"
#include "Barigate.h"
#include "Stage.h"

class GameFrame {
public:
	GameFrame();
	~GameFrame();

private:
	HINSTANCE m_hInst{ };
	HWND m_hWnd{ };
	HDC m_hDC{ };

	bool m_gameRunning{ true };

#ifdef EDIT
	bool m_editMode{ false };
	int m_playerSize{ };
	int objFlag{ };
	wstring m_objFileName{ };
	string m_createObjTag{ };
#endif
public:
	Stage* m_curStage{ };
	vector<Stage*> m_stageVec{ };//스테이지 목록

public:

#ifdef EDIT
	void Editor(POINT mousePt);
	bool IsStageNull() const;
	void SetCreateObjTag(const string& tag);
	void SetCreateObjTag(const wstring& tagW);
	string m_barigateTag{ };
#endif

	bool Init(HINSTANCE hInst, HWND hWnd);
	LRESULT CALLBACK WndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CALLBACK MouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void CALLBACK KeyboardMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool CreateNewStage(const string& backGroundKey, const wstring& backGroundFileName);

	bool CreateBullet(const string& tag, const POINT& stPt, int moveDir, float angle = 0);

	bool SetStage(int idxDest);
	bool NextStage();
	bool PrevStage();

	void StageClear(int idxDest);

	void Input(float deltaTime);
	void FrameUpdate();
	void Update(float deltaTime);
	void DeadObjectDelete();
	void Render();

	void SaveStage(const wstring& saveFileName);
	void LoadStage(const string& saveFileName, int stageNum);

};

