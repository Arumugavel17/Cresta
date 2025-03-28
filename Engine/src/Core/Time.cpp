#include "Time.hpp"
#include "Renderer/RendererCommand.hpp"

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

    float Time::GetTime()
    {
        return RenderCommand::GetTime();
    }
}
