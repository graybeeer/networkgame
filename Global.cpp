#include "Global.h"
#include "Structures.h"
#include "GameFrame.h"
#pragma warning(disable: 26451)

// 전역변수 생성

GameFrame gameFrame{ };

std::random_device rd;
std::default_random_engine dre(rd());
KEYINPUT keyInfo;

SIZE clientMinMax{ 800, 600 };
SIZE clientSize{ 800, 600 };
RECT client{ 0, 0, clientSize.cx, clientSize.cy };
bool debuging{ false };
float gravity{ 9.8f };

const string TAGMONSTER{ "Monster" };
const string TAGPLAYER{ "Player" };
const string TAGBARIGATE{ "Barigate" };
const string TAGMUSHROOM{ "Mushroom" };
const string TAGSLIME{ "Slime" };
const string TAGPORTAL{ "Portal" };
const string TAGBUTTON{ "Button" };

SIZE worldSize{ 1500, 1200 };
POINT cameraPos{ };
const float PI = std::_Pi;
const wstring saveFileName = L"StageInf.ini";
string workingDirName{ };
wstring workingDirNameW{ };

const string tSaveFile = "textureSave.tex";
const string anSaveFile = "AnimationSave.ani";

// 유용하게 쓰일 연산자 오버로딩
POINT& operator+(POINT& lhs, const POINT& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

POINT& operator-(POINT& lhs, const POINT& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	return lhs;
}

POINT operator+(const POINT& lhs, const POINT& rhs)
{
	POINT temp{ lhs };
	temp.x += rhs.x;
	temp.y += rhs.y;
	return temp;
}

POINT operator-(const POINT& lhs, const POINT& rhs)
{
	POINT temp{ lhs };
	temp.x -= rhs.x;
	temp.y -= rhs.y;
	return temp;
}

POINT& operator*(POINT& lhs, const int rhs)
{
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

// 전역함수
/* 랜덤 값 */
int GetRandInt(int min, int max) 
{
	if (min > max) {
		swap(min, max);
	}
	else if (min == max) {
		return min;
	}

	std::uniform_int_distribution<> uid{ min, max };

	return uid(dre);
}

double GetRandReal(double min, double max)
{
	if (min > max) {
		swap(min, max);
	}
	else if (min == max) {
		return min;
	}

	std::uniform_real_distribution<> urd{ min, max };

	return urd(dre);
}

/* 각도 변환 */
double Deg2Rad(double degree) 
{
	return degree * PI / 180;
}

double Deg2Rad(int degree) 
{
	return static_cast<double>(degree) * PI / 180;
}

float Deg2Rad(float degree)
{
	return degree * PI / 180;
}

double Rad2Deg(double radian)
{
	return radian * 180 / PI;
}

double Rad2Deg(int radian)
{
	return static_cast<double>(radian) * 180 / PI;
}

float Rad2Deg(float radian)
{
	return radian * 180 / PI;
}

/* 히트박스 그리기 */
void RenderHitBox(HDC destDC, const RECT& hitBox)
{
	if (debuging) {
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(destDC, hPen);
		HBRUSH hBR = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBR = (HBRUSH)SelectObject(destDC, hBR);

		RECT rc{ hitBox };

		Rectangle(destDC, rc.left, rc.top, rc.right, rc.bottom);

		hBR = (HBRUSH)SelectObject(destDC, hOldBR);
		hPen = (HPEN)SelectObject(destDC, hOldPen);
		DeleteObject(hBR);
		DeleteObject(hPen);
	}
}

void RenderHitBoxRotate(HDC destDC, const RECT& hitBox, float angle)
{
	if (debuging) {
		HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		HPEN hOldPen = (HPEN)SelectObject(destDC, hPen);
		HBRUSH hBR = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH hOldBR = (HBRUSH)SelectObject(destDC, hBR);

		RECT rc{ hitBox };

		POINT p[4]{
			{ rc.left, rc.top },
			{ rc.right, rc.top },
			{ rc.left, rc.bottom },
		};

		float resultSin = sin(angle);
		float resultCos = -cos(angle);

		SIZE sz{ rc.right - rc.left, rc.bottom - rc.top };
		POINT center{ rc.left + sz.cx / 2, rc.top + sz.cy / 2 };

		POINT rotatePt[3]{ };
		for (int i = 0; i < 3; ++i) {
			POINT pt{ p[i].x - center.x, p[i].y - center.y };

			rotatePt[i] = {
				LONG(pt.y * resultSin + pt.x * resultCos) + center.x,
				LONG(pt.y * resultCos - pt.x * resultSin) + center.y
			};

			p[i] = rotatePt[i];
		}
		
		p[3] = { { rotatePt[1].x + rotatePt[2].x - rotatePt[0].x },
			{ rotatePt[1].y + rotatePt[2].y - rotatePt[0].y } };

		swap(p[2], p[3]);

		Polygon(destDC, p, 4);

		hBR = (HBRUSH)SelectObject(destDC, hOldBR);
		hPen = (HPEN)SelectObject(destDC, hOldPen);
		DeleteObject(hBR);
		DeleteObject(hPen);
	}
}

/* 비트맵 관련 함수 */
bool ChangeColorBitmap(COLORREF* tColorArr, int arrSize, COLORREF destColor,
	const char* srcFileName, const char* saveFileName)
{
	FILE* fp;
	fopen_s(&fp, srcFileName, "rb");
	if (!fp) {
		return false;
	}

	BITMAPFILEHEADER fh{ };
	BITMAPINFOHEADER ih{ };
	fread(&fh, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&ih, sizeof(BITMAPINFOHEADER), 1, fp);

	LONG w = ih.biWidth;
	LONG h = ih.biHeight;

	if (w * h >= UINT_MAX) {
		return false;
	}

	vector<PIXEL> pixelVec;
	pixelVec.resize(w * h);
	fread(&pixelVec[0], sizeof(PIXEL), pixelVec.size(), fp);

	for (auto& c : pixelVec) {
		for (int i = 0; i < arrSize; ++i) {
			if (c == tColorArr[i]) {
				c = destColor;
			}
		}
	}

	FILE* save{ };
	fopen_s(&save, saveFileName, "wb");
	if (!save) {
		return false;
	}

	fwrite(&fh, sizeof(fh), 1, save);
	fwrite(&ih, sizeof(ih), 1, save);
	fwrite(&pixelVec[0], sizeof(PIXEL), pixelVec.size(), save);

	fclose(fp);
	fclose(save);

	return true;
}

bool ChangeColorBitmap(const vector<COLORREF>& tColorVec, COLORREF destColor,
	const char* srcFileName, const char* saveFileName)
{
	FILE* fp;
	fopen_s(&fp, srcFileName, "rb");
	if (!fp) {
		return false;
	}

	BITMAPFILEHEADER fh{ };
	BITMAPINFOHEADER ih{ };
	fread(&fh, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&ih, sizeof(BITMAPINFOHEADER), 1, fp);

	LONG w = ih.biWidth;
	LONG h = ih.biHeight;

	if (w * h >= UINT_MAX) {
		return false;
	}

	vector<PIXEL> pixelVec;
	pixelVec.resize(w * h);
	fread(&pixelVec[0], sizeof(PIXEL), pixelVec.size(), fp);

	for (auto& c : pixelVec) {
		for (auto targetColor : tColorVec) {
			if (c == targetColor) {
				c = destColor;
			}
		}
	}

	FILE* save{ };
	fopen_s(&save, saveFileName, "wb");
	if (!save) {
		return false;
	}

	fwrite(&fh, sizeof(fh), 1, save);
	fwrite(&ih, sizeof(ih), 1, save);
	fwrite(&pixelVec[0], sizeof(PIXEL), pixelVec.size(), save);

	fclose(fp);
	fclose(save);

	return true;
}

bool ReverseBitmapH(int stX, int stY, int w, int h,
	const char* srcFileName, const char* saveFileName)
{
	FILE* fp{ };
	fopen_s(&fp, "Effect002.bmp", "rb");
	BITMAPFILEHEADER bmpfh{ };
	BITMAPINFOHEADER bmpih{ };

	fread(&bmpfh, sizeof(bmpfh), 1, fp);
	fread(&bmpih, sizeof(bmpih), 1, fp);

	vector<PIXEL> pVec;
	LONG bw = bmpih.biWidth;
	LONG bh = bmpih.biHeight;

	if (w > bw or stX >= bw or stX + w > bw) {
		return false;
	}

	if (h > bh or stY >= bh or stY + h > bh) {
		return false;
	}

	pVec.resize(bw * bh);


	fread(&pVec[0], sizeof(PIXEL), pVec.size(), fp);

	PIXEL* arr = new PIXEL[bw];

	/*for (int i = 0; i < bh / 2; ++i) {
		memcpy(arr, &pVec[i * bw], sizeof(PIXEL) * bw);
		memcpy(&pVec[i * w], &pVec[(h - i - 1) * bw], sizeof(PIXEL) * w);
		memcpy(&pVec[(h - i - 1) * w], arr, sizeof(PIXEL) * bw);
	}*/

	for (int i = stY; i < h / 2; ++i) {
		memcpy(arr, &pVec[i * bw + stX], sizeof(PIXEL) * w);
		memcpy(&pVec[i * bw + stX], &pVec[(h - i - 1) * bw + stX], sizeof(PIXEL) * w);
		memcpy(&pVec[(h - i - 1) * bw + stX], arr, sizeof(PIXEL) * w);
	}

	FILE* fp2{ };
	fopen_s(&fp2, "new.bmp", "wb");
	if (!fp2) {
		return false;
	}

	fwrite(&bmpfh, sizeof(bmpfh), 1, fp2);
	fwrite(&bmpih, sizeof(bmpih), 1, fp2);
	fwrite(&pVec[0], sizeof(PIXEL), pVec.size(), fp2);

	delete[] arr;

	fclose(fp);
	fclose(fp2);

	return true;
}

bool ReverseCellBitmapH(const POINT& maxFrame, const char* srcFileName, const char* saveFileName)
{
	FILE* fp{ };
	fopen_s(&fp, "Effect002.bmp", "rb");
	BITMAPFILEHEADER bmpfh{ };
	BITMAPINFOHEADER bmpih{ };

	fread(&bmpfh, sizeof(bmpfh), 1, fp);
	fread(&bmpih, sizeof(bmpih), 1, fp);

	vector<PIXEL> pVec;
	LONG bw = bmpih.biWidth;
	LONG bh = bmpih.biHeight;

	pVec.resize(bw * bh);

	SIZE frameSize{ bw / maxFrame.x, bh / maxFrame.y };

	fread(&pVec[0], sizeof(PIXEL), pVec.size(), fp);

	PIXEL* arr = new PIXEL[frameSize.cx];
	POINT framePos{ 0, maxFrame.y - 1 };
	while (true) {
		for (int i = 0; i < frameSize.cy / 2; ++i) {
			int idxX = framePos.x * frameSize.cx;
			int idxY = i + (framePos.y * frameSize.cy);
			int destIdx = frameSize.cx + (framePos.y * frameSize.cy) - i - 1;

			memcpy(arr, &pVec[idxY * bw + idxX], sizeof(PIXEL) * frameSize.cx);
			memcpy(&pVec[idxY * bw + idxX], &pVec[destIdx * bw + idxX], sizeof(PIXEL) * frameSize.cx);
			memcpy(&pVec[destIdx * bw + idxX], arr, sizeof(PIXEL) * frameSize.cx);
		}

		framePos.x++;
		if (framePos.x == maxFrame.x) {
			framePos.y--;
			framePos.x = 0;
			if (framePos.y == -1) {
				break;
			}
		}
	}

	FILE* fp2{ };
	fopen_s(&fp2, "new.bmp", "wb");

	fwrite(&bmpfh, sizeof(bmpfh), 1, fp2);
	fwrite(&bmpih, sizeof(bmpih), 1, fp2);
	fwrite(&pVec[0], sizeof(PIXEL), pVec.size(), fp2);

	delete[] arr;

	fclose(fp);
	fclose(fp2);

	return true;
}

bool ReverseCellBitmapW(const POINT& maxFrame, const char* srcFileName, const char* saveFileName)
{
	FILE* fp{ };
	fopen_s(&fp, "Effect002.bmp", "rb");
	BITMAPFILEHEADER bmpfh{ };
	BITMAPINFOHEADER bmpih{ };

	fread(&bmpfh, sizeof(bmpfh), 1, fp);
	fread(&bmpih, sizeof(bmpih), 1, fp);

	vector<PIXEL> pVec;
	LONG bw = bmpih.biWidth;
	LONG bh = bmpih.biHeight;

	pVec.resize(bw * bh);

	SIZE frameSize{ bw / maxFrame.x, bh / maxFrame.y };

	fread(&pVec[0], sizeof(PIXEL), pVec.size(), fp);
	POINT framePos{ 0, maxFrame.y - 1 };
	while (true) {
		for (int i = 0; i < frameSize.cx / 2; ++i) {
			int idxX = i + framePos.x * frameSize.cx;
			int idxY = framePos.y * frameSize.cy;
			int destIdx = frameSize.cx + (framePos.x * frameSize.cx) - i - 1;
			for (int y = 0; y < frameSize.cy; ++y) {
				swap(pVec[(idxY + y) * bw + idxX], pVec[(idxY + y) * bw + destIdx]);
			}
		}

		framePos.x++;
		if (framePos.x == maxFrame.x) {
			framePos.y--;
			framePos.x = 0;
			if (framePos.y == -1) {
				break;
			}
		}
	}

	FILE* fp2{ };
	fopen_s(&fp2, "new.bmp", "wb");
	if (!fp2) {
		return false;;
	}

	fwrite(&bmpfh, sizeof(bmpfh), 1, fp2);
	fwrite(&bmpih, sizeof(bmpih), 1, fp2);
	fwrite(&pVec[0], sizeof(PIXEL), pVec.size(), fp2);

	fclose(fp);
	fclose(fp2);

	return true;
}

bool ReverseCellBitmap(const POINT& maxFrame, const char* srcFileName,
	const char* saveFileName, const string& wh)
{
	if (wh == "w") {
		ReverseCellBitmapW(maxFrame, srcFileName, saveFileName);
	}
	else if (wh == "h") {
		ReverseCellBitmapH(maxFrame, srcFileName, saveFileName);
	}
	else if (wh == "wh") {

	}

	return false;
}

/* 파일입출력 관련 함수들 */
bool FindFile(const wstring& addPath, const char* fileName)
{
	SetCurrentDirectory((workingDirNameW + addPath).c_str());
	FILE* fp = fopen(fileName, "r");
	if (fp == nullptr) {
		return false;
	}

	fclose(fp);
	return true;
}

bool FindFile(const char* fileName)
{
	FILE* fp = fopen(fileName, "r");
	if (fp == nullptr) {
		return false;
	}

	fclose(fp);
	return true;
}

bool GetWorkingDirPath()
{
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);

	wstring wStrPath{ path };
	workingDirName.assign(wStrPath.begin(), wStrPath.end());
	for (int i = 0; i < 3; ++i) {
		workingDirName = workingDirName.substr(0, workingDirName.find_last_of("\\/"));
	}
	workingDirNameW.assign(workingDirName.begin(), workingDirName.end());

	return true;
}

void FwriteString(const string& str, FILE* fp)
{
	if (!fp) {
		return;
	}
	else if (str.empty()) {
		return;
	}

	fwrite(str.c_str(), sizeof(char), str.size(), fp);
	fputc('\n', fp);
}

void FwriteString(const wstring& wStr, FILE* fp)
{
	if (!fp) {
		return;
	}
	else if (wStr.empty()) {
		return;
	}

	fwrite(wStr.c_str(), sizeof(wchar_t), wStr.size(), fp);
	fputwc(L'\n', fp);
}

void FreadString(string& str, FILE* fp)
{
	if (!fp) {
		return;
	}

	char buffer[MAX_BUFFER]{ 0, };
	fgets(buffer, MAX_BUFFER, fp);

	str = buffer;
	if (!str.empty()) {
		str.erase(remove(str.begin(), str.end(), '\n'));
	}
}

void FreadString(wstring& wstr, FILE* fp)
{
	if (!fp) {
		return;
	}

	wchar_t buffer[MAX_BUFFER]{ 0, };
	fgetws(buffer, MAX_BUFFER, fp);

	wstr = buffer;
	if (!wstr.empty()) {
		wstr.erase(remove(wstr.begin(), wstr.end(), L'\n'));
	}
}

void RotateImage(CImage* dest, CImage* src, float angle)
{

}

int GetCCW(const POINT& a, const POINT& b, const POINT& c)
{
	int o1 = a.x * b.y + b.x * c.y - c.x * a.y;
	int o2 = a.y * b.x + b.y * c.x - c.y * a.x;

	return o1 - o2;
}

bool GetIntersectLineToLine(const POINT& a1, const POINT& a2,
	const POINT& b1, const POINT& b2, POINT& crossPoint) 
{
	int ccwA = GetCCW(a1, a2, b1) * GetCCW(a1, a2, b2);
	int ccwB = GetCCW(b1, b2, a1) * GetCCW(b1, b2, a2);

	if (ccwA == 0 and ccwB == 0) {
		if (a1.x == b1.x) {
			return b1.y <= a2.y and a1.y <= b2.y;
		}
		else {
			return b1.x <= a2.x and a1.x <= b2.x;
		}
	}

	return ccwA <= 0 and ccwB <= 0;
}

bool OBB(const POINT& centerA, const SIZE& szA, float rotA,
	const POINT& centerB, const SIZE& szB, float rotB)
{
	float sinA = -sin(rotA);
	float cosA = cos(rotA);
	float sinB = -sin(rotB);
	float cosB = cos(rotB);

	// 모든 벡터는 0, 0을 시점으로하는 벡터임
	POINT distVec{ centerA.x - centerB.x, centerA.y - centerB.y };
	float dirVec[4][2] {
		{ (szA.cx / 2) * cosA, (szA.cy / 2) * sinA },
		{ (szA.cx / 2) * cosA, (szA.cy / 2) * sinA },
		{ (szB.cx / 2) * cosA, (szB.cy / 2) * sinA },
		{ (szB.cx / 2) * cosA, (szB.cy / 2) * sinA },
	};

	for (int i = 0; i < 4; ++i) {
		float sum{ };
		float vecSize = sqrt(dirVec[i][0] * dirVec[i][0] + dirVec[i][1] * dirVec[i][1]);
		float unitVec[2]{ dirVec[i][0] / vecSize, dirVec[i][1] / vecSize };

		for (int j = 0; j < 4; ++j) {
			sum += abs(unitVec[0] * dirVec[i][0] + unitVec[1] * dirVec[i][1]);
		}

		if (abs(distVec.x * unitVec[0] + distVec.y * unitVec[1]) > sum) {
			return false;
		}
	}

	return true;
}

bool OBB(const RECT& A, float rotA, const RECT& B, float rotB)
{
	SIZE szA{ A.right - A.left, A.bottom - A.top };
	SIZE szB{ B.right - B.left, B.bottom - B.top };

	POINT centerA{ A.left + szA.cx / 2, A.top + szA.cy / 2 };
	POINT centerB{ B.left + szB.cx / 2, B.top + szB.cy / 2 };

	float sinA = -sin(rotA);
	float cosA = cos(rotA);
	float sinB = -sin(rotB);
	float cosB = cos(rotB);

	// 모든 벡터는 0, 0을 시점으로하는 벡터임
	POINT distVec{ centerA.x - centerB.x, centerA.y - centerB.y };
	float dirVec[4][2]{
		{ (szA.cx / 2) * cos(rotA), (szA.cx / 2) * sin(rotA) },
		{ (szA.cy / 2) * cos(rotA - PI / 2), (szA.cy / 2) * sin(rotA - PI / 2) },
		{ (szB.cx / 2) * cos(rotB), (szB.cx / 2) * sin(rotB) },
		{ (szB.cy / 2) * cos(rotB - PI / 2), (szB.cy / 2) * sin(rotB - PI / 2) },
	};

	float resultSum[4]{ };
	for (int i = 0; i < 4; ++i) {
		float sum{ };
		float vecSize = sqrt(dirVec[i][0] * dirVec[i][0] + dirVec[i][1] * dirVec[i][1]);
		float unitVec[2]{ dirVec[i][0] / vecSize, dirVec[i][1] / vecSize };

		for (int j = 0; j < 4; ++j) {
			sum += abs(unitVec[0] * dirVec[j][0] + unitVec[1] * dirVec[j][1]);
		}

		resultSum[i] = sum;

		if (abs(distVec.x * unitVec[0] + distVec.y * unitVec[1]) > sum) {
			return false;
		}
	}

	return true;
}

void PlayerDead()
{
	gameFrame.SetStage(3);
}

void NextStage()
{
	gameFrame.NextStage();
}

void SetStage(int num)
{
	gameFrame.SetStage(num);
}

bool CreateBulletStage(const string& tag, const POINT& stPt, int moveDir, float angle)
{
	return gameFrame.CreateBullet(tag, stPt, moveDir, angle);
}
