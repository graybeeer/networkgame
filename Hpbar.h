#pragma once

#include "framework.h"
#include "Structures.h"

class HPBAR {
public:
	HPBAR() = delete;
	HPBAR(const POINT& objPt, const SIZE& objHitBoxSize, int maxHP, COLORREF hpColor) {
		hpBar.resize(hpBarSize.cy);
		for (unsigned i = 0; i < hpBar.size(); ++i) {
			hpBar[i].resize(hpBarSize.cx + 2);
		}
		prtPt = objPt;
		prtPt.y -= 10;
		this->maxHP = maxHP;
		this->objHitBoxSize = objHitBoxSize;

		m_hpColor.r = hpColor & 0xff;
		m_hpColor.g = (hpColor >> 8) & 0xff;
		m_hpColor.b = (hpColor >> 16) & 0xff;
	}

	~HPBAR() {
		SelectObject(bmpDC, oldBmp);
		DeleteObject(bmp);
		DeleteDC(bmpDC);
	}

private:
	HDC bmpDC{ };
	HBITMAP bmp;
	HBITMAP oldBmp{ };

	vector<vector<PIXEL>> hpBar;
	SIZE objHitBoxSize{ };
	POINT prtPt{ };
	SIZE hpBarSize{ 50, 5 };
	int maxHP{ };
	int hp{ };
	float hpPercentage{ 1.f };
	PIXEL m_hpColor{ };

public:
	void SetHpColor(COLORREF hpColor) {
		m_hpColor.r = hpColor & 0xff;
		m_hpColor.g = (hpColor >> 8) & 0xff;
		m_hpColor.b = (hpColor >> 16) & 0xff;
	}

public:
	void ResetColor() {
		for (unsigned i = 0; i < hpBar.size(); ++i) {
			for (unsigned j = 0; j < hpBar[i].size(); ++j) {
				hpBar[i][j].r = 0;
				hpBar[i][j].g = 0;
				hpBar[i][j].b = 0;
			}
		}
	}

	void Update(int hp, POINT objPt) {
		prtPt = { objPt.x, objPt.y - objHitBoxSize.cy / 2 };
		prtPt.y -= 10;

		if (hp < 0) {
			hp = 0;
		}
		hpPercentage = (hp / (float)maxHP);
		ResetColor();
		int colorwidth = static_cast<int>((hpBarSize.cx - 1) * hpPercentage);
		for (unsigned i = 1; i < hpBar.size() - 1; ++i) {
			for (unsigned j = 1; j < (unsigned)colorwidth; ++j) {
				hpBar[i][j].r = m_hpColor.r;
				hpBar[i][j].g = m_hpColor.g;
				hpBar[i][j].b = m_hpColor.b;
			}
		}
	}

	void Render(HDC destDC) {
		if (!bmpDC) {
			bmpDC = CreateCompatibleDC(destDC);
			bmp = CreateCompatibleBitmap(destDC, hpBarSize.cx + 2, hpBarSize.cy);
			oldBmp = (HBITMAP)SelectObject(bmpDC, bmp);
		}

		for (unsigned i = 1; i < hpBar.size() - 1; ++i) {
			for (unsigned j = 1; j < hpBar[i].size(); ++j) {
				SetPixel(bmpDC, j, i, RGB(hpBar[i][j].r, hpBar[i][j].g, hpBar[i][j].b));
			}
		}

		BitBlt(destDC, prtPt.x - (hpBarSize.cx + 2) / 2, prtPt.y - hpBarSize.cy / 2,
			hpBarSize.cx + 2, hpBarSize.cy, bmpDC, 0, 0, SRCCOPY);
	}
};