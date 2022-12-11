#include "Timer.h"

Timer* Timer::m_pInstance = nullptr;

Timer::Timer() : m_tick{ }, m_prev{ }, m_timeScale{ 1.f }, m_deltaTime{ } { }

Timer::~Timer() { }

Timer* Timer::GetInst() {
    if (!m_pInstance) {
        m_pInstance = new Timer;
    }

    return m_pInstance;
}

void Timer::Destroy() {
    if (m_pInstance) {
        delete m_pInstance;
    }
    m_pInstance = nullptr;
}

void Timer::SetTimeScale(float timeScale) {
    m_timeScale = timeScale;
}

float Timer::GetDeltaTime() const {
    return m_deltaTime;
}

float Timer::GetTimeScale() const {
    return m_timeScale;
}

bool Timer::Init() {
    QueryPerformanceFrequency(&m_tick);

    QueryPerformanceCounter(&m_prev);

    return true;
}

void Timer::Update() {
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);

    m_deltaTime = (current.QuadPart - m_prev.QuadPart) / (float)m_tick.QuadPart;

    m_prev = current;
}