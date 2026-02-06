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
         "[{}][{}][{}][l{}:c{}]{}",
         f_get_time(),
         LOGLEVEL_TO_STRING[std::to_underlying(level)],
         sl.file_name(),
         sl.line(),
         sl.column(),
         message
      );
   }
   return std::format(
      "[{}][{}]{}",
      f_get_time(), LOGLEVEL_TO_STRING[std::to_underlying(level)], message
   );
}

std::string Logger::f_get_time() {
   auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
   return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}
