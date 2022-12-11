#include "Portal.h"

Portal::Portal()
{
	AnimationManager::GetInst()->AddNewAnimation("PortalAnimation", L"BluePortal.png", POINT{ 5, 2 });

	m_animation = AnimationManager::GetInst()->GetAnimation("PortalAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 9, 0.2f);

	m_curClip = m_animation->GetClip("default");
}

Portal::Portal(const POINT& pt)
{

	m_pt = pt;
	AnimationManager::GetInst()->AddNewAnimation("PortalAnimation", L"BluePortal.png", POINT{ 5, 2 });
	
	m_animation = AnimationManager::GetInst()->GetAnimation("PortalAnimation");
	m_animation->AddNewClip("default", POINT{ 0, 0 }, 9, 0.2f);

	m_curClip = m_animation->GetClip("default");

}

Portal::~Portal()
{
	if (m_animation) {
		delete m_animation;
	}
	m_animation = nullptr;
}

POINT Portal::GetPt() const
{
	return m_pt;
}

RECT Portal::GetRC() const
{
	RECT rc{ m_pt.x - m_hitBoxSize.cx / 2, m_pt.y - m_hitBoxSize.cy / 2,
		m_pt.x + m_hitBoxSize.cx / 2, m_pt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

SIZE Portal::GetSize() const
{
	return SIZE();
}

void Portal::Update(float deltaTime)
{
	if (m_animation and m_curClip) {
		m_curClip->Update(deltaTime);
	}
}

void Portal::Render(HDC destDC)
{
	RenderHitBox(destDC, GetRC());

	if (!m_img and !m_animation) {
		return;
	}
	else if (m_img) {
		m_img->SetTransparentColor(RGB(0, 0, 0));
		m_img->TransparentBlt(destDC, GetRC());
	}
	else if (m_animation) {
		m_animation->Render(destDC, GetRC(), m_curClip->GetImgRC(), RGB(0,0,0));
	}
}

void Portal::Collision()
{
}

bool Portal::Save(FILE* fp)
{
	fwrite(&m_pt, sizeof(POINT), 1, fp);
	fwrite(&m_imgSize, sizeof(SIZE), 1, fp);
	fwrite(&m_hitBoxSize, sizeof(SIZE), 1, fp);
	return true;
}

bool Portal::Load(FILE* fp)
{
	fread(&m_pt, sizeof(POINT), 1, fp);
	fread(&m_imgSize, sizeof(SIZE), 1, fp);
	fread(&m_hitBoxSize, sizeof(SIZE), 1, fp);
	return true;
}
