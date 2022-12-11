#include "Bullet.h"
#include "AnimationManager.h"
#include "Global.h"

Bullet::Bullet(const string& tag, const POINT& pt, const SIZE& size, int moveDir, float moveAngle) {
	AnimationManager::GetInst()->AddNewAnimation("BulletAnimation", L"BulletBlue.png", POINT{ 8, 8 });

	m_bulletPt = pt;
	m_bulletSize = size;
	m_strTag = tag;

	if (moveDir == MD_ANGLE) {
		m_moveAngle = true;
		m_angle = moveAngle;
	}
	else if (moveDir == MD_TARGET) {
		m_targeting = true;
	}
	else {
		m_moveDir = moveDir;
	}

	m_animation = AnimationManager::GetInst()->GetAnimation("BulletAnimation");

	m_animation->AddNewClip("default", POINT{ 0, 0 }, 60, 0.05f);
	m_curClip = m_animation->GetClip("default");
}

Bullet::Bullet(const string& tag, const POINT& pt, int moveDir, float moveAngle) {
	AnimationManager::GetInst()->AddNewAnimation("BulletAnimation", L"BulletBlue.png", POINT{ 8, 8 });

	m_bulletPt = pt;
	m_moveDir = moveDir;
	m_strTag = tag;

	if (moveDir == MD_ANGLE) {
		m_moveAngle = true;
		m_angle = moveAngle;
	}
	else if (moveDir == MD_TARGET) {
		m_targeting = true;
	}
	else {
		m_moveDir = moveDir;
	}

	m_animation = AnimationManager::GetInst()->GetAnimation("BulletAnimation");

	m_animation->AddNewClip("default", POINT{ 0, 0 }, 60, 0.05f);
	m_curClip = m_animation->GetClip("default");
}

Bullet::~Bullet() {
	if (m_animation) {
		delete m_animation;
	}
	m_animation = nullptr;
}

RECT Bullet::GetRenderRC() const {
	return RECT{ m_bulletPt.x - m_bulletSize.cx / 2, m_bulletPt.y - m_bulletSize.cy / 2,
		m_bulletPt.x + m_bulletSize.cx / 2, m_bulletPt.y + m_bulletSize.cy / 2 };
}

POINT Bullet::GetBulletPt() const {
	return m_bulletPt;
}

SIZE Bullet::GetBulletSize() const {
	return m_bulletSize;
}

void Bullet::SetBulletPt(const POINT& pt) {
	m_bulletPt = pt;
}

bool Bullet::GetTargetingMode() const {
	return m_targeting;
}

const string& Bullet::GetTag() const {
	return m_strTag;
}

RECT Bullet::GetBulletRC() const {
	RECT rc{ m_bulletPt.x - m_hitBoxSize.cx / 2, m_bulletPt.y - m_hitBoxSize.cy / 2,
		m_bulletPt.x + m_hitBoxSize.cx / 2, m_bulletPt.y + m_hitBoxSize.cy / 2 };
	return rc;
}

bool Bullet::IsAlive() const {
	return m_alive;
}

float Bullet::GetAngle() const {
	return m_angle;
}

void Bullet::CheckOut() {
	if (m_bulletPt.x + m_hitBoxSize.cx / 2 <= 0) {
		Die();
	}
	else if (m_bulletPt.x - m_hitBoxSize.cx / 2 >= worldSize.cx) {
		Die();
	}

	if (m_bulletPt.y + m_hitBoxSize.cx / 2 <= 0) {
		Die();
	}
	else if (m_bulletPt.y - m_hitBoxSize.cx / 2 >= worldSize.cx) {
		Die();
	}
}

void Bullet::Update(float deltaTime, const POINT& targetPt) {
	m_curClip->Update(deltaTime);
	float width = (static_cast<float>(targetPt.x - m_bulletPt.x));
	float height = (static_cast<float>(targetPt.y - m_bulletPt.y));
	
	m_angle = atan2(height, width);

	MoveAngle(deltaTime, m_angle);
	m_time += deltaTime;
	if (m_limitTime <= m_time) {
		Die();
	}

	CheckOut();
}

void Bullet::Update(float deltaTime) {
	m_curClip->Update(deltaTime);
	if (m_moveAngle or m_targeting) {
		MoveAngle(deltaTime, m_angle);
	}
	else {
		Move(deltaTime);
	}

	CheckOut();
}

void Bullet::Render(HDC destDC) {
	if (m_bulletPt.x - m_hitBoxSize.cx / 2 > cameraPos.x + clientSize.cx / 2) {
		return;
	}
	else if (m_bulletPt.x + m_hitBoxSize.cx / 2 < cameraPos.x - clientSize.cx / 2) {
		return;
	}

	if (m_bulletPt.y - m_hitBoxSize.cy / 2 > cameraPos.y + clientSize.cy / 2) {
		return;
	}
	else if (m_bulletPt.y + m_hitBoxSize.cy / 2 < cameraPos.y - clientSize.cy / 2) {
		return;
	}

	POINT imgPt = m_curClip->GetFramePos();
	SIZE imgSize = m_curClip->GetFrameSize();


	if (!m_moveAngle and !m_targeting) {
		m_animation->Render(destDC, GetRenderRC(), m_curClip->GetImgRC(), RGB(0, 0, 0));
		RenderHitBox(destDC, GetBulletRC());
	}
	else {
		m_animation->RenderRotate(destDC, GetRenderRC(), m_curClip->GetImgRC(), m_curClip->GetFrameSize(), 
			m_angle);
		RenderHitBoxRotate(destDC, GetBulletRC(), m_angle);
		//m_animation->Render(destDC, GetRenderRC(), m_curClip->GetImgRC(), RGB(0, 0, 0));
	}
}

void Bullet::Move(float deltaTime) {
 	switch (m_moveDir) {
	case MD_UP:
		MoveUp(deltaTime);
		break;

	case MD_DOWN:
		MoveDown(deltaTime);
		break;

	case MD_LEFT:
		MoveLeft(deltaTime);
		break;

	case MD_RIGHT:
		MoveRight(deltaTime);
		break;
	}
}

void Bullet::MoveLeft(float deltaTime) {
	int dist = static_cast<int>(m_speed * deltaTime);
	m_bulletPt.x -= dist;
	m_moveDist += dist;
	if (m_moveDist >= m_limitMoveDist) {
		Die();
	}
}

void Bullet::MoveRight(float deltaTime) {
	int dist = static_cast<int>(m_speed * deltaTime);
	m_bulletPt.x += dist;
	m_moveDist += dist;
	if (m_moveDist >= m_limitMoveDist) {
		Die();
	}
}

void Bullet::MoveUp(float deltaTime) {
	int dist = static_cast<int>(m_speed * deltaTime);
	m_bulletPt.y -= dist;
	m_moveDist += dist;
	if (m_moveDist >= m_limitMoveDist) {
		Die();
	}
}

void Bullet::MoveDown(float deltaTime) {
	int dist = static_cast<int>(m_speed * deltaTime);
	m_bulletPt.y += dist;
	m_moveDist += dist;
	if (m_moveDist >= m_limitMoveDist) {
		Die();
	}
}

void Bullet::MoveToTarget(float deltaTime, const POINT& targetPt) {
	if (targetPt.x < m_bulletPt.x) {
		m_bulletPt.x -= static_cast<int>(round(m_speed / 2) * deltaTime);
	}
	else {
		m_bulletPt.x += static_cast<int>(round(m_speed / 2) * deltaTime);
	}

	if (targetPt.y < m_bulletPt.y) {
		m_bulletPt.y -= static_cast<int>(round(m_speed / 2) * deltaTime);
	}
	else {
		m_bulletPt.y += static_cast<int>(round(m_speed / 2) * deltaTime);
	}
}

void Bullet::MoveAngle(float deltaTime, int angle) {
	m_bulletPt.x += static_cast<int>(round(sin(angle) * (m_speed / 4) * deltaTime));
	m_bulletPt.y += static_cast<int>(round(cos(angle) * (m_speed / 4) * deltaTime));
}

void Bullet::MoveAngle(float deltaTime, float angle) {
	//float dist1 = cos(angle) * m_speed * deltaTime;
	//float dist2 = sin(angle) * m_speed * deltaTime;
	m_bulletPt.x += static_cast<int>(round(cosf(angle) * (m_speed / 4) * deltaTime));
	m_bulletPt.y += static_cast<int>(round(sinf(angle) * (m_speed / 4) * deltaTime));
}

void Bullet::Die() {
	m_alive = false;
}

bool Bullet::Save(FILE* fp) {
	return true;
}

bool Bullet::Load(FILE* fp) {
	return true;
}