#include "Barigate.h"

Barigate::Barigate()
{
}

Barigate::Barigate(const POINT& pt, const SIZE& size) {
	m_pt = pt;
	m_imgSize = size;
	m_hitBoxSize = size;
}

Barigate::Barigate(const string& imgTag, const POINT& pt) {
	m_imgTag = imgTag;
	m_img = TextureManager::GetInst()->GetTexture(m_imgTag);
	m_pt = pt;
	m_imgSize = SIZE{ m_img->GetWidth(), m_img->GetHeight() };
	m_hitBoxSize = m_imgSize;
}

Barigate::Barigate(const string& imgTag, const POINT& pt, const SIZE& size) {
	m_imgTag = imgTag;
	m_img = TextureManager::GetInst()->GetTexture(m_imgTag);
	m_pt = pt;
	m_imgSize = SIZE{ m_img->GetWidth(), m_img->GetHeight() };
	m_hitBoxSize = m_imgSize;
}

Barigate::~Barigate() {

}

POINT Barigate::GetPt() const { 
	return m_pt;
}

RECT Barigate::GetRC() const {
	RECT rc{ m_pt.x - m_hitBoxSize.cx / 2, m_pt.y - m_hitBoxSize.cy / 2, 
		m_pt.x + m_hitBoxSize.cx / 2, m_pt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

SIZE Barigate::GetSize() const {
	return SIZE();
}

void Barigate::Update(float deltaTime) {
	if (m_animation and m_curClip) {
		m_curClip->Update(deltaTime);
	}
}

void Barigate::Render(HDC destDC) {
	RenderHitBox(destDC, GetRC());

	if (!m_img and !m_animation) {
		return;
	}
	else if (m_img) {
		m_img->SetTransparentColor(RGB(255, 255, 255));
		m_img->TransparentBlt(destDC, GetRC());
	}
	else if (m_animation) {

	}
}

void Barigate::Collision() {

}

bool Barigate::Save(FILE* fp) {
	fwrite(&m_pt, sizeof(POINT), 1, fp);
	fwrite(&m_imgSize, sizeof(SIZE), 1, fp);
	if (m_imgTag == "") {
		m_imgTag = "NULL";
	}

	FwriteString(m_imgTag, fp);

	return true;
}

bool Barigate::Load(FILE* fp) {
	fread(&m_pt, sizeof(POINT), 1, fp);
	fread(&m_imgSize, sizeof(SIZE), 1, fp);
	m_hitBoxSize = m_imgSize;
	FreadString(m_imgTag, fp);
	if (m_imgTag == "NULL") {
		m_imgTag = "";
		m_img = nullptr;
	}
	else {
		m_img = TextureManager::GetInst()->GetTexture(m_imgTag);
	}

	return true;
}