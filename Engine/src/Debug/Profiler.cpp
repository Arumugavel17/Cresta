#include "Profiler.hpp"

#include <mutex>

namespace Cresta
{
	namespace Profiler
	{
		BenchMarker* BenchMarker::sm_Instance = nullptr;

		void BenchMarker::BeginSession(const std::string& name, const std::string& filepath)
		{
			if (m_CurrentSession)
			{
				// If there is already a current session, then close it before beginning new one.
				// Subsequent profiling output meant for the original session will end up in the
				// newly opened session instead.  That's better than having badly formatted
				// profiling output.
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					CRESTA_CORE_INFO("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);
				}
				InternalEndSession();
			}

			m_OutputStream.open(filepath);

			if (m_OutputStream.is_open())
			{
				m_CurrentSession = new InstrumentationSession(name);
				WriteHeader();
			}
			else
			{
				if (Log::GetCoreLogger()) // Edge case: BeginSession() might be before Log::Init()
				{
					CRESTA_CORE_INFO("Instrumentor could not open results file '{0}'.", filepath);
				}
			}
		}

		void BenchMarker::EndSession()
		{
			InternalEndSession();
		}

		void BenchMarker::WriteProfile(const ProfileResult& result)
		{
			std::stringstream json;

			json << std::setprecision(3) << std::fixed;
			json << ",{";
			json << "\"cat\":\"function\",";
			json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
			json << "\"name\":\"" << result.Name << "\",";
			json << "\"ph\":\"X\",";
			json << "\"pid\":0,";
			json << "\"tid\":" << result.ThreadID << ",";
			json << "\"ts\":" << result.Start.count();
			json << "}";

			if (m_CurrentSession)
			{
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		BenchMarker& BenchMarker::Get()
		{
			if (sm_Instance)
			{
				return *sm_Instance;
			}
			static BenchMarker instance;
			sm_Instance = &instance;
			return instance;
		}

		BenchMarker::BenchMarker()
		{

		}

		BenchMarker::~BenchMarker()
		{
			EndSession();
		}

		void BenchMarker::WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
			m_OutputStream.flush();
		}

		void BenchMarker::WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		void BenchMarker::InternalEndSession()
		{
			if (m_CurrentSession)
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}
	}
}