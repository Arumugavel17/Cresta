#include "Time.hpp"

namespace Cresta
{

    float Time::s_Time = 0;
    void Time::SetDeltaTime(float time)
    {
        s_Time = time;
    }

    float Time::DeltaTime()
    {
        return s_Time;
    }
}
