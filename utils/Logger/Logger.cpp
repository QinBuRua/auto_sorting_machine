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

void Logger::set_log_file(const std::string& filename, const std::source_location& sl) {
   m_FileName = filename;
   m_Fout.open(filename);
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message(
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

std::string Logger::get_log_file() {
   return m_FileName;
}

LogLevel Logger::get_log_level() {
   return m_LogLevel;
}

void Logger::auto_initialize(LogLevel level, const std::source_location& sl) {
   m_LogLevel = level;
   auto now   = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
   m_FileName = std::format("logs\\{:%Y-%m-%d %H.%M.%S}.txt", now);
   m_Fout.open(m_FileName);
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message(
            LogLevel::ERROR,
            std::format("Fail to open log file \"{}\"", m_FileName),
            sl
         )
      };
   }
}

void Logger::log(LogLevel level, const std::string& message, const std::source_location& sl) {
   if (level < m_LogLevel) {
      return;
   }
   m_Fout << f_make_message(level, message, sl) << std::endl;
   if (m_Fout.fail()) {
      throw std::runtime_error{
         f_make_message(
            LogLevel::ERROR,
            std::format("Fail to log to file \"{}\"", m_FileName),
            sl
         )
      };
   }
}

std::string Logger::m_FileName{};

std::ofstream Logger::m_Fout{};

LogLevel Logger::m_LogLevel;

std::string Logger::f_make_message(LogLevel level, const std::string& message, const std::source_location& sl) {
   if (m_LogLevel == LogLevel::DEBUG) {
      return std::format(
         "[{}][{}][{}][l{}:c{}] {}",
         f_get_time(),
         LOGLEVEL_TO_STRING[std::to_underlying(level)],
         sl.file_name(),
         sl.line(),
         sl.column(),
         message
      );
   }
   return std::format(
      "[{}][{}] {}",
      f_get_time(), LOGLEVEL_TO_STRING[std::to_underlying(level)], message
   );
}

std::string Logger::f_get_time() {
   auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
   return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}

log::Tag::Tag(const std::source_location& sl) {
   level    = LogLevel::DEBUG;
   location = sl;
}

log::Tag::Tag(LogLevel lvl, const std::source_location& sl) {
   level    = lvl;
   location = sl;
}

void log::debug(const std::string& message, const std::source_location& sl) {
   if (Logger::get_log_file().empty()) {
      Logger::auto_initialize();
   }
   Logger::log(LogLevel::DEBUG, message, sl);
}

void log::info(const std::string& message, const std::source_location& sl) {
   if (Logger::get_log_file().empty()) {
      Logger::auto_initialize();
   }
   Logger::log(LogLevel::INFO, message, sl);
}

void log::warn(const std::string& message, const std::source_location& sl) {
   if (Logger::get_log_file().empty()) {
      Logger::auto_initialize();
   }
   Logger::log(LogLevel::WARN, message, sl);
}

void log::error(const std::string& message, const std::source_location& sl) {
   if (Logger::get_log_file().empty()) {
      Logger::auto_initialize();
   }
   Logger::log(LogLevel::ERROR, message, sl);
}

void log::fatal(const std::string& message, const std::source_location& sl) {
   if (Logger::get_log_file().empty()) {
      Logger::auto_initialize();
   }
   Logger::log(LogLevel::FATAL, message, sl);
}
