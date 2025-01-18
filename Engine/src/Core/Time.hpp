#pragma once

namespace Cresta
{
	class Time
	{
	public:
		static void SetDeltaTime(float time);
		static float DeltaTime();
	private:
		static float s_Time;
	};
}