#include "pch.h"
#include "Logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <ostream>

std::shared_ptr<spdlog::logger> Sphynx::Logger::InternalLogger;
std::shared_ptr<spdlog::logger> Sphynx::Logger::ClientLogger;



void Sphynx::Logger::Init()
{
	InternalLogger = std::make_shared<spdlog::logger>("Sphynx", std::make_shared<LoggerSink>());
	InternalLogger->set_pattern("%^[%n::%l](%!thread id:%t)%v%$");
	InternalLogger->set_level(spdlog::level::trace);
	ClientLogger = std::make_shared<spdlog::logger>("Application", std::make_shared<LoggerSink>());
	ClientLogger->set_pattern("%^[%n::%l](%!thread id:%t)%v%$");
	ClientLogger->set_level(spdlog::level::trace);
}

void Sphynx::Logger::LoggerSink::sink_it_(const spdlog::details::log_msg& msg)
{
	spdlog::memory_buf_t formatted;
	base_sink<std::mutex>::formatter_->format(msg, formatted);
	auto str = fmt::to_string(formatted);
	Events::GlobalEventSystem::GetInstance()->QueueEvent<OnLog>(OnLog(str, msg.level));
}

void Sphynx::Logger::LoggerSink::flush_()
{
	Events::GlobalEventSystem::GetInstance()->QueueEvent<OnLogFlush>(OnLogFlush());
}
