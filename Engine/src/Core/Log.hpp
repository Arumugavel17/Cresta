#pragma once
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <set>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

namespace Cresta 
{
	class Log
	{
	public:
		static void Init();
		static void Shutdown();

		static Ref<spdlog::logger>& GetCoreLogger() { return sm_CoreLogger; }
		static Ref<spdlog::logger>& GetClientLogger() { return sm_ClientLogger; }
	private:
		static Ref<spdlog::logger> sm_CoreLogger;
		static Ref<spdlog::logger> sm_ClientLogger;
	};

}

template<typename OStream, typename T>
inline OStream& operator<<(OStream& os, const std::vector<T> vec)
{
	for (auto& i : vec)
	{
		os << "[" << i << "] ";
	}
	os << "\n";
	return os;
}

template<typename OStream, typename T>
inline OStream& operator<<(OStream& os, const std::set<T> vec)
{
	for (auto& i : vec)
	{
		os << "[" << i << "] ";
	}
	os << "\n";
	return os;
}

template<typename OStream, typename T>
inline OStream& operator<<(OStream& os, const std::unordered_set<T> vec)
{
	for (auto& i : vec)
	{
		os << "[" << i << "] ";
	}
	os << "\n";
	return os;
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

// Core log macros
#define CRESTA_CORE_TRACE(...)    ::Cresta::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CRESTA_CORE_INFO(...)     ::Cresta::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CRESTA_CORE_WARN(...)     ::Cresta::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CRESTA_CORE_ERROR(...)    ::Cresta::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CRESTA_CORE_CRITICAL(...) ::Cresta::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CRESTA_CORE_LOG_WITH_TIME(...) \
    ::Cresta::Log::GetCoreLogger()->info("[{}] {}", std::chrono::system_clock::now(), fmt::format(__VA_ARGS__))

// Client log macros
#define CRESTA_TRACE(...)         ::Cresta::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CRESTA_INFO(...)          ::Cresta::Log::GetClientLogger()->info(__VA_ARGS__)
#define CRESTA_WARN(...)          ::Cresta::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CRESTA_ERROR(...)         ::Cresta::Log::GetClientLogger()->error(__VA_ARGS__)
#define CRESTA_CRITICAL(...)      ::Cresta::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CRESTA_LOG_WITH_TIME(...) \
    ::Cresta::Log::GetClientLogger()->info("[{}] {}", std::chrono::system_clock::now(), fmt::format(__VA_ARGS__))

#define CRESTA_ASSERT(x, ...) \
    if (x) { CRESTA_ERROR("Assertion Failed: " __VA_ARGS__); __debugbreak(); }