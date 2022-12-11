#include "Slime.h"

Slime::Slime() : Monster() {
	AnimationManager::GetInst()->AddNewAnimation("SlimeAnimation", L"Slime.png", POINT{ 8, 3 });
	m_tag = TAGSLIME;

	m_animation = AnimationManager::GetInst()->GetAnimation("SlimeAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 4, 0.2f);
	m_animation->AddNewClip("move", POINT{ 4, 0 }, 4, 0.2f);
	m_animation->AddNewClip("attacked", POINT{ 5, 1 }, 4, 0.2f);
	m_animation->AddNewClip("die", POINT{ 1, 2 }, 4, 0.2f);

	SetColorKey(RGB(0, 0, 0));

	m_curClip = m_animation->GetClip("default");
}

Slime::Slime(const POINT& stPt) : Monster(stPt) {
	AnimationManager::GetInst()->AddNewAnimation("SlimeAnimation", L"Slime.png", POINT{ 8, 3 });
	m_tag = TAGSLIME;

	m_animation = AnimationManager::GetInst()->GetAnimation("SlimeAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 4, 0.2f);
	m_animation->AddNewClip("move", POINT{ 4, 0 }, 4, 0.2f);
	m_animation->AddNewClip("attacked", POINT{ 5, 1 }, 4, 0.2f);
	m_animation->AddNewClip("die", POINT{ 1, 2 }, 4, 0.2f);

	SetColorKey(RGB(0, 0, 0));

	m_curClip = m_animation->GetClip("default");
}

Slime::Slime(const POINT& stPt, int moveDir) : Monster(stPt, moveDir) {
	AnimationManager::GetInst()->AddNewAnimation("SlimeAnimation", L"Slime.png", POINT{ 8, 3 });
	m_tag = TAGSLIME;

	m_animation = AnimationManager::GetInst()->GetAnimation("SlimeAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 4, 0.2f);
	m_animation->AddNewClip("move", POINT{ 4, 0 }, 4, 0.2f);
	m_animation->AddNewClip("attacked", POINT{ 5, 1 }, 4, 0.2f);
	m_animation->AddNewClip("die", POINT{ 1, 2 }, 4, 0.2f);

	SetColorKey(RGB(0, 0, 0));

	m_curClip = m_animation->GetClip("default");
}

Slime::~Slime() {

}