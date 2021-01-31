#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace Sphynx {
	//Abstract for now(must call init). Thinking about making this class a singlton(no need to call init). 
	class Logger final{
	private:
		static std::shared_ptr<spdlog::logger> InternalLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger> GetInternalLogger() { return InternalLogger; };
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return ClientLogger; };

	};
}

#define Core_Error(...)         ::Sphynx::Logger::GetInternalLogger()->error(__VA_ARGS__)
#define Core_Warn(...)          ::Sphynx::Logger::GetInternalLogger()->warn(__VA_ARGS__)
#define Core_Info(...)          ::Sphynx::Logger::GetInternalLogger()->info(__VA_ARGS__)
#define Core_Trace(...)         ::Sphynx::Logger::GetInternalLogger()->trace(__VA_ARGS__)
#define Core_Debug_Message(...) ::Sphynx::Logger::GetInternalLogger()->debug(__VA_ARGS__)
								
#define Client_Error(...)       ::Sphynx::Logger::GetClientLogger()->error(__VA_ARGS__)
#define Client_Warn(...)        ::Sphynx::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define Client_Info(...)        ::Sphynx::Logger::GetClientLogger()->info(__VA_ARGS__)
#define Client_Trace(...)       ::Sphynx::Logger::GetClientLogger()->trace(__VA_ARGS__)