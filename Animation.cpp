#include "Animation.h"
#include "Global.h"

struct FVec2 {
    float x{ };
    float y{ };

    FVec2() { };
    FVec2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    FVec2(long x, long y) {
        this->x = x;
        this->y = y;
    }

    FVec2(const FVec2& other) {
        x = other.x;
        y = other.y;
    }
    ~FVec2() { };

    float VecSize() {
        float size = sqrt((x * x) + (y * y));
        return size;
    }

    FVec2& Normalize() {
        float size = VecSize();
        x /= size;
        y /= size;
        return *this;
    }

    FVec2& Rotate(float angle) {
        float c = cosf(angle);
        float s = -sinf(angle);
        x = x * c - y * s;
        y = x * s + y * c;
        return *this;
    }

    operator POINT() {
        POINT newPt = { x, y };
        return newPt;
    }

    FVec2& operator-=(const FVec2& other) {
        
    }
};

Animation::Animation() { }

Animation::~Animation() {
    map<string, AnimationClip*>::iterator iter = m_clipMap.begin();
    map<string, AnimationClip*>::iterator iterEnd = m_clipMap.end();

    for (; iter != iterEnd; ++iter) {
        if (iter->second) {
            delete iter->second;
        }

        iter->second = nullptr;
    }

    m_clipMap.clear();
}

Animation::Animation(const Animation& other) {
    m_srcImg = other.m_srcImg;
    m_srcFileName = other.m_srcFileName;
    m_maxFrameLength = other.m_maxFrameLength;
    CopyClip(&other);
}

POINT Animation::GetMaxFrameLength() const {
    return m_maxFrameLength;
}

wstring Animation::GetSrcFileName() const {
    return m_srcFileName;
}

bool Animation::LoadSrcImg(CImage* srcImg, const wstring& fileName, POINT maxFrameLength) {
    m_srcImg = srcImg;

    m_srcFileName = fileName;
    m_maxFrameLength = maxFrameLength;

    return true;
}

bool Animation::AddNewClip(const string& clipName, POINT startFrame,
    int frameLength, float frameTime) {
    if (!m_srcImg) {
        return false;
    }

    map<string, AnimationClip*>::iterator iter = m_clipMap.find(clipName);
    if (iter == m_clipMap.end()) {
        AnimationClip* newClip = new AnimationClip;
        if (!newClip->CreateNewClip(m_srcImg, startFrame, frameLength, m_maxFrameLength, frameTime)) {
            delete newClip;
            newClip = nullptr;
            return false;
        }

        m_clipMap.insert(make_pair(clipName, newClip));
    }

    return true;
}

bool Animation::AddNewClip(const string& clipName, POINT startFrame,
    int frameLength, SIZE frameSize, float frameTime) {
    if (!m_srcImg) {
        return false;
    }

    map<string, AnimationClip*>::iterator iter = m_clipMap.find(clipName);
    if (iter == m_clipMap.end()) {
        AnimationClip* newClip = new AnimationClip;
        if (!newClip->CreateNewClip(m_srcImg, startFrame, frameLength, m_maxFrameLength, frameSize, frameTime)) {
            delete newClip;
            newClip = nullptr;
            return false;
        }

        m_clipMap.insert(make_pair(clipName, newClip));
    }

    return true;
}

bool Animation::CopyClip(const Animation* protoAnimation) {
    map<string, AnimationClip*>::const_iterator iterEnd = protoAnimation->m_clipMap.end();
    for (auto iter = protoAnimation->m_clipMap.begin(); iter != iterEnd; ++iter) {
        m_clipMap.insert(make_pair(iter->first, new AnimationClip(*(iter->second))));
    }

    return true;
}

AnimationClip* Animation::GetClip(const string& clipName) {
    map<string, AnimationClip*>::iterator iter = m_clipMap.find(clipName);

    if (iter == m_clipMap.end()) {
        return NULL;
    }

    return (*iter).second;
}

HDC Animation::GetDC() const {
    return m_srcImg->GetDC();
}

void Animation::ReleaseDC() const {
    m_srcImg->ReleaseDC();
}

void Animation::Render(HDC destDC, const RECT& destRC, const RECT& srcRC, COLORREF colorKey) {
    m_srcImg->TransparentBlt(destDC, destRC, srcRC, colorKey);
}

void Animation::Render(HDC destDC, const RECT& destRC, const RECT& srcRC, bool rev, COLORREF colorKey) {
    
}

//void Animation::Render(HDC destDC, const SIZE& destSZ,
//    const RECT& destRC, const RECT& srcRC, bool rev, COLORREF colorKey) {
//    CImage temp;
//    temp.Create(destSZ.cx, destSZ.cy, m_srcImg->GetBPP());
//
//    RECT rc{ rev ? destSZ.cx - 1 : 0, 0, rev ? 0 : destSZ.cx, destSZ.cy };
//    m_srcImg->StretchBlt(temp.GetDC(), rc, srcRC, SRCCOPY);
//    temp.ReleaseDC();
//
//    temp.TransparentBlt(destDC, destRC, RECT{ 1, 0, destSZ.cx, destSZ.cy }, colorKey);
//}
//
//void Animation::RenderRotate(HDC destDC, const RECT& destRC, const RECT& srcRC,
//    const SIZE& srcSz, float angle, COLORREF colorKey) {
//    POINT centerSrc{ srcRC.left + srcSz.cx / 2, srcRC.top + srcSz.cy / 2 };
//
//    POINT destPt[3]{
//        { srcRC.left, srcRC.top },
//        { srcRC.right, srcRC.top },
//        { srcRC.left, srcRC.bottom }
//    };
//
//    float resultSin = sin(angle);
//    float resultCos = -cos(angle);
//
//    POINT rotatePt[3]{ };
//    for (int i = 0; i < 3; ++i) {
//		POINT pt{ destPt[i].x - centerSrc.x, destPt[i].y - centerSrc.y };
//        
//        rotatePt[i] = {
//            /*LONG(pt.x * resultCos - pt.y * resultSin),
//            LONG(pt.x * resultSin + pt.y * resultCos)*/
//            LONG(pt.y * resultSin + pt.x * resultCos) + srcSz.cx / 2,
//            LONG(pt.y * resultCos - pt.x * resultSin) + srcSz.cy / 2
//        };
//    }
//
//    CImage rotateImg;
//    rotateImg.Create(srcSz.cx, srcSz.cy, m_srcImg->GetBPP());
//
//    m_srcImg->PlgBlt(rotateImg.GetDC(), rotatePt, srcRC);
//    rotateImg.ReleaseDC();
//
//    //rotateImg.BitBlt(destDC, destRC, POINT{ 0, 0 }, SRCCOPY);
//    rotateImg.TransparentBlt(destDC, destRC, RECT{ 0, 0, srcSz.cx, srcSz.cy }, colorKey);
//}

void Animation::Render(HDC destDC, const SIZE& destSZ, const RECT& destRC,
    const RECT& srcRC, bool rev, COLORREF colorKey, int alphaBlend) {
    SIZE srcSZ{ srcRC.right - srcRC.left, srcRC.bottom - srcRC.top };

    CImage temp;
    temp.Create(srcSZ.cx, srcSZ.cy, m_srcImg->GetBPP());

    RECT rc{ rev ? srcSZ.cx - 1 : 0, 0, rev ? 0 : srcSZ.cx, srcSZ.cy };
    m_srcImg->StretchBlt(temp.GetDC(), rc, srcRC, SRCCOPY);
    temp.ReleaseDC();

    if (alphaBlend != 0xff) {
        CImage alpha{ };
        alpha.Create(srcSZ.cx, srcSZ.cy, temp.GetBPP());
        int left = destRC.left <= 0 ? -destRC.left : destRC.left;
        TransparentBlt(alpha.GetDC(), 0, 0, srcSZ.cx, srcSZ.cy, destDC, left, destRC.top,
            destSZ.cx, destSZ.cy, colorKey);
        alpha.ReleaseDC();

        temp.TransparentBlt(alpha.GetDC(), 0, 0, srcSZ.cx, srcSZ.cy, 0, 0, srcSZ.cx, srcSZ.cy, colorKey);
        //temp.AlphaBlend(alpha.GetDC(), 0, 0, alphaBlend, AC_SRC_OVER);
        alpha.ReleaseDC();

        //m_srcImg->AlphaBlend(temp.GetDC(), rc, srcRC, alphaBlend, AC_SRC_OVER);
        alpha.AlphaBlend(destDC, destRC, RECT{ 1, 0, srcSZ.cx, srcSZ.cy }, alphaBlend, AC_SRC_OVER);
        //alpha.TransparentBlt(destDC, destRC, RECT{ 1, 0, srcSZ.cx, srcSZ.cy }, colorKey);
    }
    else {
        SIZE srcSZ{ srcRC.right - srcRC.left, srcRC.bottom - srcRC.top };

        CImage temp;
        temp.Create(srcSZ.cx, srcSZ.cy, m_srcImg->GetBPP());

        RECT rc{ rev ? srcSZ.cx - 1 : 0, 0, rev ? 0 : srcSZ.cx, srcSZ.cy };
        m_srcImg->StretchBlt(temp.GetDC(), rc, srcRC, SRCCOPY);
        temp.ReleaseDC();

        temp.TransparentBlt(destDC, destRC, RECT{ 1, 0, srcSZ.cx, srcSZ.cy }, colorKey);
    }
}

void Animation::RenderRotate(HDC destDC, const RECT& destRC, const RECT& srcRC,
    const SIZE& srcSz, float angle, COLORREF colorKey, int alphaBlend) {
    POINT centerSrc{ srcRC.left + srcSz.cx / 2, srcRC.top + srcSz.cy / 2 };

    POINT destPt[3]{
        { srcRC.left, srcRC.top },
        { srcRC.right, srcRC.top },
        { srcRC.left, srcRC.bottom }
    };

    float resultSin = -sin(angle);
    float resultCos = -cos(angle);

    POINT rotatePt[3]{ };
    for (int i = 0; i < 3; ++i) {
        POINT pt{ destPt[i].x - centerSrc.x, destPt[i].y - centerSrc.y };

        rotatePt[i] = {
            /*LONG(pt.x * resultCos - pt.y * resultSin),
            LONG(pt.x * resultSin + pt.y * resultCos)*/
            LONG(pt.x * resultCos + pt.y * resultSin) + srcSz.cx / 2,
            LONG(pt.x * resultSin - pt.y * resultCos) + srcSz.cy / 2
        };
    }

    CImage rotateImg;
    rotateImg.Create(srcSz.cx, srcSz.cy, m_srcImg->GetBPP());

    m_srcImg->PlgBlt(rotateImg.GetDC(), rotatePt, srcRC);
    rotateImg.ReleaseDC();

    //rotateImg.BitBlt(destDC, destRC, POINT{ 0, 0 }, SRCCOPY);

    /*CImage temp{ };
    temp.Create(srcSz.cx, srcSz.cy, rotateImg.GetBPP());

    rotateImg.AlphaBlend(temp.GetDC(), 0, 0, alphaBlend, AC_SRC_OVER);
    temp.ReleaseDC();*/

    rotateImg.TransparentBlt(destDC, destRC, RECT{ 0, 0, srcSz.cx, srcSz.cy }, colorKey);
}
