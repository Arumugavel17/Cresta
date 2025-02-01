#pragma once
#include "Crestaph.hpp"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <sstream>

namespace Cresta
{
	namespace Profiler
	{
		using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

		struct ProfileResult
		{
			std::string Name;

			FloatingPointMicroseconds Start;
			std::chrono::microseconds ElapsedTime;
			std::thread::id ThreadID;
		};

		struct InstrumentationSession
		{
			std::string Name;
			
			InstrumentationSession(std::string name) : Name(name)
			{}

			~InstrumentationSession()
			{
			}
		};

		class BenchMarker
		{
		public:
			BenchMarker();
			~BenchMarker();
			BenchMarker(const BenchMarker&) = delete;
			BenchMarker(BenchMarker&&) = delete;

			void BeginSession(const std::string& name, const std::string& filepath = "C:\\dev\\Cresta\\results.json");
			void EndSession();
			void WriteProfile(const ProfileResult& result);
			static BenchMarker& Get();

			void WriteHeader();
			void WriteFooter();
			void InternalEndSession();

		private:
			static BenchMarker* sm_Instance;
			InstrumentationSession* m_CurrentSession;
			std::ofstream m_OutputStream;
		};

		template <size_t N>
		struct ChangeResult
		{
			char Data[N];
		};

		class InstrumentationTimer
		{
		public:
			InstrumentationTimer(const char* name)
				: m_Name(name), m_Stopped(false)
			{
				m_StartTimepoint = std::chrono::steady_clock::now();
			}

			~InstrumentationTimer()
			{
				if (!m_Stopped)
				{
					Stop();
				}
			}

			void Stop()
			{
				auto endTimepoint = std::chrono::steady_clock::now();
				auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
				auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() - std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

				BenchMarker::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

				m_Stopped = true;
			}
		private:
			const char* m_Name;
			std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
			bool m_Stopped;
		};

		template <size_t N, size_t K>
		constexpr auto CleanupOutputString(const char(&expr)[N], const char(&remove)[K])
		{
			ChangeResult<N> result = {};

			size_t srcIndex = 0;
			size_t dstIndex = 0;
			while (srcIndex < N)
			{
				size_t matchIndex = 0;
				while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
				{
					matchIndex++;
				}

				if (matchIndex == K - 1)
				{
					srcIndex += matchIndex;
				}
				result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
				srcIndex++;
			}
			return result;
		}
	}
}

#define CRESTA_PROFILE 1
#if CRESTA_PROFILE 
#define CRESTA_PROFILE_BEGIN_SESSION(name, filepath) ::Cresta::Profiler::BenchMarker::Get().BeginSession(name, filepath)
#define CRESTA_PROFILE_END_SESSION() ::Cresta::Profiler::BenchMarker::Get().EndSession()
#define CRESTA_PROFILE_SCOPE_LINE2(name, line) constexpr auto fixedName##line = ::Cresta::Profiler::CleanupOutputString(name, "__cdecl ");\
											   ::Cresta::Profiler::InstrumentationTimer timer##line(fixedName##line.Data)
#define CRESTA_PROFILE_SCOPE_LINE(name, line) CRESTA_PROFILE_SCOPE_LINE2(name, line)
#define CRESTA_PROFILE_SCOPE(name) CRESTA_PROFILE_SCOPE_LINE(name, __LINE__)
#define CRESTA_PROFILE_FUNCTION() CRESTA_PROFILE_SCOPE(__FUNCSIG__)
#else
#define CRESTA_PROFILE_BEGIN_SESSION(name, filepath)
#define CRESTA_PROFILE_END_SESSION()
#define CRESTA_PROFILE_SCOPE(name)
#define CRESTA_PROFILE_FUNCTION()
#endif