#include "GameFrame.h"
#include "Timer.h"
#include "Input.h"
#include "Global.h"
#include "BadOrange.h"
#include "Slime.h"
#include "EffectManager.h"
#include "Portal.h"
#include "Button.h"
#include "Server.h"

GameFrame::GameFrame() : m_hInst{ }, m_hWnd{ }, m_hDC{ }, m_gameRunning{ true } { }

GameFrame::~GameFrame() {
    Timer::Destroy();
    TextureManager::Destroy();
    AnimationManager::Destroy();
    EffectManager::Destroy();

    vector<Stage*>::iterator iterEnd = m_stageVec.end();
    for (auto iter = m_stageVec.begin(); iter != iterEnd; ++iter) {
        if ((*iter)) {
            delete (*iter);
        }
        (*iter) = nullptr;
    }
    m_stageVec.clear();
}

#ifdef EDIT
void GameFrame::Editor(POINT mousePt) {
    switch (objFlag) {
    case 1:
        if (m_playerSize >= 1) {
            break;
        }
        break;

    case 2:
        break;

    case 3:
        break;
    }
}

bool GameFrame::IsStageNull() const { 
    if (m_curStage) {
        return false;
    }
    return true;
}

void GameFrame::SetCreateObjTag(const string& tag) {
    m_createObjTag = tag;
}

void GameFrame::SetCreateObjTag(const wstring& tagW) {
    m_createObjTag.assign(tagW.begin(), tagW.end());
}
#endif

bool GameFrame::Init(HINSTANCE hInst, HWND hWnd) {
    SetCurrentDirectory(workingDirNameW.c_str());

    m_hInst = hInst;
    m_hWnd = hWnd;
    if (!m_hInst or !m_hWnd) {
        return false;
    }

    m_hDC = GetDC(m_hWnd);
    if (!m_hDC) {
        return false;
    }

    if (!Timer::GetInst()->Init()) {
        return false;
    }

    if (!TextureManager::GetInst()->Init(m_hInst, m_hDC, L"BackGround2.bmp")) {
        return false;
    }

    if (!AnimationManager::GetInst()->Init(m_hInst)) {
        return false;
    }

    EffectManager::GetInst()->LoadFromeFile("EffectInf.txt");
    /*AnimationManager::GetInst()->AddNewAnimation("Skill1", L"AxeSlashEffect.png", POINT{ 9, 1 });
    AnimationManager::GetInst()->AddNewAnimation("Effect001", L"Effect001.bmp", POINT{ 6, 1 });
    AnimationManager::GetInst()->AddNewAnimation("Effect002", L"Effect002.png", POINT{ 5, 5 });*/
    
    TextureManager::GetInst()->Load();
    for (int i = 0; ; ++i) {
        string fileName{ "Stage" + to_string(i) + ".map" };
        if (!FindFile(fileName.c_str())) {
            break;
        }
		LoadStage(fileName, i);
    }
    SetStage(0);
   
    return true;
}

LRESULT GameFrame::WndMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_COMMAND:
        break;

    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
        MouseMessage(hWnd, message, wParam, lParam);
        break;

    case WM_KEYDOWN:
    case WM_KEYUP:
        KeyboardMessage(hWnd, message, wParam, lParam);
        break;

    default:
        break;
    }

    return ((LRESULT)0);
}

void GameFrame::MouseMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
#ifdef EDIT
    static bool dlag{ false };
    static POINT dlagSt{ };
#endif

    switch (message) {
    //case WM_MOUSEMOVE:
    //    if (dlag) {
    //        POINT mousePt = { cameraPos.x - clientSize.cx / 2 + LOWORD(lParam),
    //            cameraPos.y - clientSize.cy / 2 + HIWORD(lParam) };
    //        m_curStage->CalCameraPos(mousePt);
    //    }
    //    break;

    case WM_LBUTTONDOWN:
        if (m_curStage->GetStageNum() == 0) {
            NextStage();
        }
#ifdef EDIT
        {
            if (m_createObjTag == "") {
                break;
            }

		    POINT mousePt = { cameraPos.x - clientSize.cx / 2 + LOWORD(lParam),
			    cameraPos.y - clientSize.cy / 2 + HIWORD(lParam) };

            if (m_createObjTag == TAGPLAYER) {
<<<<<<< HEAD
<<<<<<< HEAD
                m_curStage->AddPlayer(new Player{ true,0, mousePt });
=======
                m_curStage->AddPlayer(new Player{ true, mousePt });
>>>>>>> 96b5bd690f54986b92f87682e24ee4b734381c60
=======
                m_curStage->AddPlayer(new Player{ true, mousePt });
>>>>>>> 96b5bd690f54986b92f87682e24ee4b734381c60
            }
            else if (m_createObjTag == TAGMUSHROOM) {
                m_curStage->AddMonster(new BadOrange{ mousePt });
            }
            else if (m_createObjTag == TAGSLIME) {
                m_curStage->AddMonster(new Slime{ mousePt });
            }
            else if (m_createObjTag == TAGPORTAL) {
                m_curStage->AddPortal(new Portal{ mousePt });
            }
            else if (m_createObjTag == TAGBUTTON) {
                m_curStage->AddButton(new Button{ mousePt });
            }
            else if (m_createObjTag == TAGBARIGATE) {
                if (m_barigateTag == "") {
                    dlag = true;
                    dlagSt = mousePt;
                }
                else {
                    m_curStage->AddBarigate(new Barigate{ m_barigateTag, mousePt });
                }
            }
        }
#endif
        break;

#ifdef EDIT
    case WM_LBUTTONUP:
        if (dlag) {
			POINT edPt = { cameraPos.x - clientSize.cx / 2 + LOWORD(lParam),
			cameraPos.y - clientSize.cy / 2 + HIWORD(lParam) };
            POINT middle = { dlagSt.x + (edPt.x - dlagSt.x) / 2, dlagSt.y + (edPt.y - dlagSt.y) / 2 };
            SIZE sz = { abs(edPt.x - dlagSt.x), abs(edPt.y - dlagSt.y) };
			m_curStage->AddBarigate(new Barigate{ middle, sz });
            dlag = false;
            dlagSt = { };
        }
        break;

    case WM_RBUTTONDOWN:
        {
		POINT mousePt = { cameraPos.x - clientSize.cx / 2 + LOWORD(lParam),
				cameraPos.y - clientSize.cy / 2 + HIWORD(lParam) };

		m_curStage->DeleteObjEdit(mousePt);
        }
        break;
#endif
    }
}

void GameFrame::KeyboardMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE:
            break;
        }
        break;

    case WM_KEYUP:
        switch (wParam) {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        }
        break;
    }

}

bool GameFrame::CreateNewStage(const string& backGroundKey, const wstring& backGroundFileName) {
    m_curStage = new Stage{ INT_MAX, backGroundFileName };

    return true;
}

bool GameFrame::CreateBullet(const string& tag, const POINT& stPt, int moveDir, float angle) {
    if (!m_curStage) {
        return false;
    }

    if (m_curStage->GetStageNum() == 0) {
        return false;
    }

	m_curStage->CreateBullet(tag, stPt, moveDir, angle);
}

bool GameFrame::SetStage(int idxDest) {
    PlaySound(NULL, NULL, NULL);
    EffectManager::GetInst()->Reset();
    m_curStage = m_stageVec.at(idxDest);
    return false;
}

bool GameFrame::NextStage() {
    vector<Stage*>::iterator iterEnd = m_stageVec.end();
    for (auto iter = m_stageVec.begin(); iter != iterEnd; ++iter) {
        if (*iter == m_curStage) {
            if ((iter + 1) != iterEnd) {
				EffectManager::GetInst()->Reset();
                PlaySound(NULL, NULL, NULL);
                m_curStage = *(iter + 1);
                if (m_curStage->GetStageNum() == 1) {
                    PlaySound(L"BGM0.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC | SND_NODEFAULT);
                }
                else if (m_curStage->GetStageNum() == 2) {
                    PlaySound(L"BGM1.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC | SND_NODEFAULT);
                }
                break;
            }
        }
    }

    return false;
}

bool GameFrame::PrevStage() {
    vector<Stage*>::reverse_iterator iterEnd = m_stageVec.rend();
    for (auto iter = m_stageVec.rbegin(); iter != iterEnd; ++iter) {
        if (*iter == m_curStage) {
            if ((iter + 1) != iterEnd) {
				EffectManager::GetInst()->Reset();
                PlaySound(NULL, NULL, NULL);
                m_curStage = *(iter + 1);
                break;
            }
        }
    }

    return false;
}

void GameFrame::StageClear(int idxDest) {
    int num = m_stageVec[idxDest]->GetStageNum();
    if (m_stageVec[idxDest]) {
        delete m_stageVec[idxDest];
    }
    m_stageVec[idxDest] = new Stage;
    m_stageVec[idxDest]->Load(string{ "Stage" + to_string(idxDest) + ".map" });
}

void GameFrame::Input(float deltaTime) {
    keyInfo.Input(deltaTime);

#ifdef _DEBUG
    if (keyInfo[VK_CONTROL].pressed) {
        if (keyInfo[VK_F2].down) {
            //StageClear(m_curStage->GetStageNum());
            NextStage();
        } 
        else if (keyInfo[VK_F3].down) {
            //StageClear(m_curStage->GetStageNum());
            PrevStage();
        }
    }
#endif
    if (m_curStage->GetStageNum() == 0) {
        for (int i = 0; i < MAX_BUFFER; ++i) {
            if (keyInfo[i].down) {
                NextStage();
            }
        }
    }

    if (m_curStage) {
        m_curStage->Input(deltaTime);
    }
}

void GameFrame::FrameUpdate() {
	Timer::GetInst()->Update();
	float deltaTime = Timer::GetInst()->GetDeltaTime();

    Input(deltaTime);
    Update(deltaTime);
    DeadObjectDelete();
    Render();
}

void GameFrame::Update(float deltaTime) {
    if (m_curStage) {
        m_curStage->Update(deltaTime);
    }
}

void GameFrame::DeadObjectDelete() {
    if (m_curStage) {
        m_curStage->DeleteDeadObject();
    }
}

void GameFrame::Render() {
    if (m_curStage) {
        m_curStage->Render(m_hDC);
    }
}

void GameFrame::SaveStage(const wstring& saveFileName) {
    SetCurrentDirectory(workingDirNameW.c_str());

    string saveFile{ };
    saveFile.assign(saveFileName.begin(), saveFileName.end());

    m_curStage->Save(saveFile);
}

void GameFrame::LoadStage(const string& saveFileName, int stageNum) {
    SetCurrentDirectory(workingDirNameW.c_str());

    if (!m_curStage) {
        m_curStage = new Stage{ };
    }
    m_curStage->Load(saveFileName);
    m_curStage->SetStageNum(stageNum);
    m_stageVec.push_back(m_curStage);
    sort(m_stageVec.begin(), m_stageVec.end(), [](Stage* s1, Stage* s2)->bool {
        return s1->GetStageNum() < s2->GetStageNum();
        });
    m_curStage = nullptr;
}
