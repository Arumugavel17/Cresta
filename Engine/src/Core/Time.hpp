#pragma once

namespace Cresta
{
	class Application;
	
	class Time
	{
	public:
		static float DeltaTime();
	private:
		static void SetDeltaTime(float time);
		static float s_Time;

		friend class Application;
	};
}