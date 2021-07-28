#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"
#include <memory>

namespace Sphynx {
	struct OnLog : public Events::Event {
		std::string msg;
		spdlog::level::level_enum level;
		OnLog(std::string& str, spdlog::level::level_enum lvl) : msg(str), level(lvl) {};
	};
	struct OnLogFlush : public Events::Event {
		OnLogFlush() {};
	};
	//SPD uses malloc for strings, this will continue allocation after window closing 
	class Logger final{
	public:
		//Custom Sink That uses events to dispatch logs.(Should it Be Private?) 
		//TODO : Variants of this Where it logs in a file or something and dispatching events(?)
<<<<<<< HEAD
		class LoggerSink : public spdlog::sinks::base_sink<std::mutex>{
=======
		class LoggerSink final : public spdlog::sinks::base_sink<std::mutex>{
>>>>>>> Dev_ComponentSystem
		private:
			Events::EventSystem eventsystem = *Events::GlobalEventSystem::GetInstance();
		public:
			LoggerSink() {};
			//If desiring a specific EventsSystem. Inaccesible.
			LoggerSink(Events::EventSystem evs) { eventsystem = evs; };
			//We Can send the entire log_msg wrapped. but we are only sending the formatted message.
			virtual void sink_it_(const spdlog::details::log_msg& msg) override;
			virtual void flush_() override;
		};
		typedef std::shared_ptr<LoggerSink> LoggerSinkPtr;
	private:
		static std::shared_ptr<spdlog::logger> InternalLogger;
		static std::shared_ptr<spdlog::logger> ClientLogger;
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger> GetInternalLogger() { return InternalLogger; };
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return ClientLogger;};

		//inline static LoggerSinkPtr GetInternalLoggerSink() { return std::make_shared<LoggerSink>(InternalLogger->sinks()[0].get()); };
		//inline static LoggerSinkPtr GetClientLoggerSink() { return std::make_shared<LoggerSink>(ClientLogger->sinks()[0].get()); };
	};
}
#ifdef DEBUG
#define Core_Error(...)         ::Sphynx::Logger::GetInternalLogger()->error(__VA_ARGS__)
#define Core_Warn(...)          ::Sphynx::Logger::GetInternalLogger()->warn(__VA_ARGS__)
#define Core_Info(...)          ::Sphynx::Logger::GetInternalLogger()->info(__VA_ARGS__)
#define Core_Trace(...)         ::Sphynx::Logger::GetInternalLogger()->trace(__VA_ARGS__)
#define Core_Debug_Message(...) ::Sphynx::Logger::GetInternalLogger()->debug(__VA_ARGS__)
#define Core_Print_Memory(ptr)  

#define Client_Error(...)       ::Sphynx::Logger::GetClientLogger()->error(__VA_ARGS__)
#define Client_Warn(...)        ::Sphynx::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define Client_Info(...)        ::Sphynx::Logger::GetClientLogger()->info(__VA_ARGS__)
#define Client_Trace(...)       ::Sphynx::Logger::GetClientLogger()->trace(__VA_ARGS__)
#else
#define Core_Error(...)        
#define Core_Warn(...)         
#define Core_Info(...)         
#define Core_Trace(...)        
#define Core_Debug_Message(...)
#define Core_Print_Memory(ptr) 

#define Client_Error(...)      
#define Client_Warn(...)       
#define Client_Info(...)       
#define Client_Trace(...)      
#endif