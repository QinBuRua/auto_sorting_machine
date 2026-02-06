//
// Created by QinBu_Rua on 2026/2/6.
//

#include "Logger.h"

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

void Logger::log(LogLevel level, const std::string& message, const std::source_location& sl) {
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

std::string Logger::f_make_message(LogLevel level, const std::string& message, const std::source_location& sl) {
   return std::format(
      "[{}][{}][l{}:c{}]{}",
      LOGLEVEL_TO_STRING[std::to_underlying(level)], sl.file_name(), sl.line(), sl.column(), message
   );
}
