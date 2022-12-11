#include "Object.h"

Object::Object() {
}

Object::Object(const POINT& pt, const SIZE& sz, const SIZE& hbSz, const string& tag) {
}

Object::~Object() {
}

string Object::GetTag() const {
	return m_tag;
}

POINT Object::GetPt() const {
	return m_pt;
}

SIZE Object::GetSize() const {
	return m_size;
}

RECT Object::GetHitBox() const {
	return RECT{ m_pt.x - m_hitBoxSize.cx / 2, m_pt.y - m_hitBoxSize.cy / 2,
		m_pt.x + m_hitBoxSize.cx / 2, m_pt.y + m_hitBoxSize.cy / 2 };
}

RECT Object::GetRC() const {
	return RECT{ m_pt.x - m_size.cx / 2, m_pt.y - m_size.cy / 2, 
		m_pt.x + m_size.cx / 2, m_pt.y + m_size.cy / 2 };
}

void Object::SetPt(const POINT& newPt) {
	m_pt = newPt;
}

void Object::SetDefaultInfo(const POINT& pt, const SIZE& sz, const SIZE& hbSz) {
	m_pt = pt; 
	m_size = sz;
	m_hitBoxSize = hbSz;
}

void Object::Input(float deltaTime) {
	
}
