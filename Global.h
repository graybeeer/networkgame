#pragma once

#include "framework.h"
#include "Input.h"
#include "Effect.h"

#ifdef _DEBUG
#define EDITOR 1
#endif

#if EDITOR
#define EDIT 1
#endif
// 전역변수로 쓰일 변수들 모음
/* 랜덤 */
#define MD_ANGLE 0xf0
#define MD_TARGET 0xff

enum class LINESTATE {
	LST_CROSS,
	LST_EQUALL,
	LST_PARALLEL
};

extern class GameFrame gameFrame;

extern std::random_device rd;
extern std::default_random_engine dre;

extern const float PI;

/* 키입력 관리 */
extern KEYINPUT keyInfo;

/* 클라이언트&월드 크기 */
extern SIZE clientMinMax;
extern SIZE clientSize;
extern RECT client;
extern SIZE worldSize;

/* */
extern bool debuging;
extern float gravity;
extern POINT cameraPos;

/* 오브젝트 태그 */
extern const string TAGPLAYER;
extern const string TAGBARIGATE;
extern const string TAGMUSHROOM;
extern const string TAGSLIME;
extern const string TAGPORTAL;
extern const string TAGBUTTON;

/* 파일 */
extern string workingDirName;
extern wstring workingDirNameW;
extern const string tSaveFile;
extern const string anSaveFile;

// 전역 함수

/* 랜덤 값 */
extern int GetRandInt(int min, int max);
extern double GetRandReal(double min, double max);

/* 각도 변환 함수 */

extern double Rad2Deg(double radian);
extern double Rad2Deg(int radian);
extern float Rad2Deg(float radian);
extern double Deg2Rad(double degree);
extern double Deg2Rad(int degree);
extern float Deg2Rad(float degree);

extern void RenderHitBox(HDC destDC, const RECT& hitBox);
extern void RenderHitBoxRotate(HDC destDC, const RECT& hitBox, float angle);
//
//extern bool ChangeColorBitmap(COLORREF* tColorArr, int arrSize, COLORREF destColor,
//	const char* srcFileName, const char* saveFileName);
//extern bool ChangeColorBitmap(const vector<PIXEL>& tColorVec, COLORREF destColor, 
//	const char* srcFileName, const char* saveFileName);

//extern Effect* CreateEffect(const POINT* stPts, const int* effectLengths, const float* frameTimes,
//	SIZE* effectImgSizes, int numOfClip);

/* 파일 입출력 관련 함수 */
extern bool GetWorkingDirPath();
extern bool FindFile(const char* fileName);
extern void FwriteString(const string& str, FILE* fp);
extern void FwriteString(const wstring& wStr, FILE* fp);
extern void FreadString(string& str, FILE* fp);
extern void FreadString(wstring& wstr, FILE* fp);

// OBB 충돌처리
extern bool OBB(const POINT& centerA, const SIZE& szA, float rotA,
	const POINT& centerB, const SIZE& szB, float rotB);
extern bool OBB(const RECT& A, float rotA, const RECT& B, float rotB);

// 스테이지 상호작용
extern bool CreateBulletStage(const string& tag, const POINT& stPt, int moveDir, float angle = 0);
extern void PlayerDead();
extern void NextStage();
extern void SetStage(int num);

template <typename T>
T GetMax(T arg, ...)
{
	T maxVal{ };
	va_list ap;
	va_start(ap, arg);

	maxVal = arg;
	arg = va_arg(ap, T);

	while (arg != T{ }) {
		if (maxVal < arg) {
			maxVal = arg;
		}
		arg = va_arg(ap, T);
	}

	va_end(ap);

	return maxVal;
}

template <typename T>
T GetMin(T arg, ...)
{
	T minVal{ };
	va_list ap;
	va_start(ap, arg);

	minVal = arg;
	arg = va_arg(ap, T);

	while (arg != T{ }) {
		if (minVal > arg) {
			minVal = arg;
		}
		arg = va_arg(ap, T);
	}

	va_end(ap);

	return minVal;
}