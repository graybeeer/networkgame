#include "BadOrange.h"

BadOrange::BadOrange() : Monster() {
	AnimationManager::GetInst()->AddNewAnimation("MonsterAnimation", L"BadOrangeMushroom.bmp", POINT{ 4, 4 });
	m_tag = TAGMUSHROOM;

	m_animation = AnimationManager::GetInst()->GetAnimation("MonsterAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 2, 0.3f);
	m_animation->AddNewClip("move", POINT{ 0, 1 }, 3, 0.2f);
	m_animation->AddNewClip("jump", POINT{ 0, 1 }, 3, 0.3f);
	m_animation->AddNewClip("attacked", POINT{ 2, 2 }, 1, 1.f);
	m_animation->AddNewClip("die", POINT{ 0, 3 }, 4, 0.3f);

	SetColorKey(RGB(255, 0, 255));

	m_curClip = m_animation->GetClip("default");
}

BadOrange::BadOrange(const POINT& startPt) : Monster(startPt) {
	AnimationManager::GetInst()->AddNewAnimation("MonsterAnimation", L"BadOrangeMushroom.bmp", POINT{ 4, 4 });
	m_tag = TAGMUSHROOM;

	m_animation = AnimationManager::GetInst()->GetAnimation("MonsterAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 2, 0.3f);
	m_animation->AddNewClip("move", POINT{ 0, 1 }, 3, 0.2f);
	m_animation->AddNewClip("jump", POINT{ 0, 1 }, 3, 0.3f);
	m_animation->AddNewClip("attacked", POINT{ 2, 2 }, 1, 1.f);
	m_animation->AddNewClip("die", POINT{ 0, 3 }, 4, 0.3f);

	SetColorKey(RGB(255, 0, 255));

	m_curClip = m_animation->GetClip("default");
}

BadOrange::BadOrange(const POINT& startPt, int moveDir) : Monster(startPt, moveDir) {
	AnimationManager::GetInst()->AddNewAnimation("MonsterAnimation", L"BadOrangeMushroom.bmp", POINT{ 4, 4 });
	m_tag = TAGMUSHROOM;

	m_animation = AnimationManager::GetInst()->GetAnimation("MonsterAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 2, 0.3f);
	m_animation->AddNewClip("move", POINT{ 0, 1 }, 3, 0.2f);
	m_animation->AddNewClip("jump", POINT{ 0, 1 }, 3, 0.3f);
	m_animation->AddNewClip("attacked", POINT{ 2, 2 }, 1, 1.f);
	m_animation->AddNewClip("die", POINT{ 0, 3 }, 4, 0.3f);

	SetColorKey(RGB(255, 0, 255));

	m_curClip = m_animation->GetClip("default");
}

BadOrange::~BadOrange() {

}
