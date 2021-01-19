#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Sphynx::Logger::InternalLogger;
std::shared_ptr<spdlog::logger> Sphynx::Logger::ClientLogger;

void Sphynx::Logger::Init()
{
	InternalLogger = spdlog::stdout_color_mt("Sphynx");
	InternalLogger->set_pattern("%^[%n](%!thread id:%t)%v%$");
	InternalLogger->set_level(spdlog::level::trace);
	ClientLogger = spdlog::stdout_color_mt("Application");
	ClientLogger->set_pattern("%^[%n](%!thread id:%t)%v%$");
	ClientLogger->set_level(spdlog::level::trace);
}
