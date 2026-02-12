//
// Created by QinBu_Rua on 2026/2/6.
//

#include "Logger.h"

#include <chrono>
#include <fstream>
#include <utility>

using namespace QinBuRua::auto_sorting_machine::utils;

static const std::string LOGLEVEL_TO_STRING[5] = {
   "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

Logger& Logger::instance() {
   static Logger loggerInstance{};
   return loggerInstance;
}

void Logger::set_log_file(const std::string& filename, const std::source_location& sl) {
   m_FileName = filename;
   m_Fout.open(filename);
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message_sl(
            LogLevel::ERROR,
            std::format("Fail to open log file \"{}\"", filename),
            sl
         )
      };
   }
}

void Logger::set_log_level(LogLevel level) noexcept {
   m_LogLevel = level;
}

void Logger::set_sl_level(LogLevel level) noexcept {
   m_WithSl = level;
}

std::string Logger::get_log_file() const {
   return m_FileName;
}

LogLevel Logger::get_log_level() const {
   return m_LogLevel;
}

LogLevel Logger::get_sl_level() const {
   return m_WithSl;
}

void Logger::auto_initialize(LogLevel level, const std::source_location& sl) {
   m_LogLevel = level;
   auto now   = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
   m_FileName = std::format("logs\\{:%Y-%m-%d %H.%M.%S}.txt", now);
   m_Fout.open(m_FileName);
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message_sl(
            LogLevel::ERROR,
            std::format("Fail to open log file \"{}\"", m_FileName),
            sl
         )
      };
   }
}

std::string Logger::log_sl(LogLevel level, const std::string& message, const std::source_location& sl) {
   if (level < m_LogLevel) {
      return {};
   }
   std::string result = f_make_message_sl(level, message, sl);
   m_Fout << result << std::endl;
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message_sl(
            LogLevel::ERROR,
            std::format("Fail to log to file \"{}\"", m_FileName),
            sl
         )
      };
   }
   return std::move(result);
}

std::string Logger::log(LogLevel level, const std::string& message) {
   if (level < m_LogLevel) {
      return {};
   }
   std::string result = f_make_message(level, message);
   m_Fout << result;
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message(
            LogLevel::ERROR,
            std::format("Fail to log to file \"{}\"", m_FileName)
         )
      };
   }
   return result;
}

Logger::Logger() {
   auto_initialize();
}

std::string Logger::f_make_message(LogLevel level, const std::string& message) {
   return std::format(
      "[{}][{}] {}",
      f_get_time_str(), LOGLEVEL_TO_STRING[std::to_underlying(level)], message
   );
}

std::string Logger::f_make_message_sl(LogLevel level, const std::string& message, const std::source_location& sl) {
   if (m_LogLevel == LogLevel::DEBUG) {
      return std::format(
         "[{}][{}][{}][l{}:c{}] {}",
         f_get_time_str(),
         LOGLEVEL_TO_STRING[std::to_underlying(level)],
         sl.file_name(),
         sl.line(),
         sl.column(),
         message
      );
   }
   return std::format(
      "[{}][{}] {}",
      f_get_time_str(), LOGLEVEL_TO_STRING[std::to_underlying(level)], message
   );
}

std::string Logger::f_get_time_str() {
   auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
   return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

log::Tag::Tag(const std::source_location& sl) {
   level    = LogLevel::WARN;
   location = sl;
}

log::Tag::Tag(LogLevel lvl, const std::source_location& sl) {
   level    = lvl;
   location = sl;
}

void log::debug_sl(const std::string& message, const std::source_location& sl) {
   Logger::instance().log_sl(LogLevel::DEBUG, message, sl);
}

void log::info_sl(const std::string& message, const std::source_location& sl) {
   Logger::instance().log_sl(LogLevel::INFO, message, sl);
}

void log::warn_sl(const std::string& message, const std::source_location& sl) {
   Logger::instance().log_sl(LogLevel::WARN, message, sl);
}

void log::error_sl(const std::string& message, const std::source_location& sl) {
   Logger::instance().log_sl(LogLevel::ERROR, message, sl);
}

void log::fatal_sl(const std::string& message, const std::source_location& sl) {
   Logger::instance().log_sl(LogLevel::FATAL, message, sl);
}
