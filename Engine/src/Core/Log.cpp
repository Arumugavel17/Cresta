

#include "Crestaph.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Cresta 
{
	Ref<spdlog::logger> Log::sm_CoreLogger = nullptr;
	Ref<spdlog::logger> Log::sm_ClientLogger = nullptr;

	void Log::Shutdown()
	{
		CRESTA_TRACE("Shutting down Log");
		spdlog::shutdown();
	}

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Cresta_Engine.log", true));

		sm_CoreLogger = std::make_shared<spdlog::logger>("Cresta", begin(logSinks), end(logSinks));
		spdlog::register_logger(sm_CoreLogger);
		sm_CoreLogger->set_level(spdlog::level::trace);
		sm_CoreLogger->flush_on(spdlog::level::trace);

		sm_ClientLogger = std::make_shared<spdlog::logger>("Application", begin(logSinks), end(logSinks));
		spdlog::register_logger(sm_ClientLogger);
		sm_ClientLogger->set_level(spdlog::level::trace);
		sm_ClientLogger->flush_on(spdlog::level::trace);
	}
}
