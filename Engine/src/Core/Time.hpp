#pragma once

namespace Cresta
{
	class Time
	{
	public:
		static void SetDeltaTime(float time);
		static float DeltaTime();
		static float GetTime();
	private:
		static float s_Time;
	};
}