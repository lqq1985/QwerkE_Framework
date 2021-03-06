#include "Time.h"

namespace QwerkE {

    double Time::m_Delta = 0.0;
    double Time::m_CurrentFrame = 0.0;
    double Time::m_LastFrame = 0.0;

    void Time::NewFrame()
    {
        m_LastFrame = m_CurrentFrame;
        m_CurrentFrame = Now();
        m_Delta = m_CurrentFrame - m_LastFrame;
    }

    double Time::Now()
    {
        unsigned __int64 freq;
        unsigned __int64 time;
        QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
        QueryPerformanceCounter((LARGE_INTEGER*)&time);
        return (double)time / freq;
    }

}
