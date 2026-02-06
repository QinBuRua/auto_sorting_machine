//
// Created by QinBu_Rua on 2026/2/6.
//

#ifndef AUTO_SORTING_MACHINE_LOGGER_H
#define AUTO_SORTING_MACHINE_LOGGER_H

#include <cstdint>
#include <source_location>
#include <string>

namespace QinBuRua::auto_sorting_machine::utils {

enum class LogLevel:uint8_t {
   DEBUG,
   INFO,
   WARN,
   ERROR,
   FATAL
};

class Logger {
public:
   static void set_log_file(
      const std::string& filename,
      const std::source_location& sl = std::source_location::current()
   );
   static void set_log_level(LogLevel level) noexcept; //只有debug等级会包含源码信息

   static std::string get_log_file();
   static LogLevel get_log_level();

   static void auto_initialize(
      LogLevel level                 = LogLevel::INFO,
      const std::source_location& sl = std::source_location::current()
   );

   static void log(
      LogLevel level,
      const std::string& message,
      const std::source_location& sl = std::source_location::current()
   );

   Logger()                         = delete;
   Logger(const Logger&)            = delete;
   Logger(Logger&&)                 = delete;
   Logger& operator=(const Logger&) = delete;
   Logger& operator=(Logger&&)      = delete;

private:
   static std::string m_FileName;
   static std::ofstream m_Fout;
   static LogLevel m_LogLevel;

private:
   static std::string f_make_message(LogLevel level, const std::string& message, const std::source_location& sl);
   static std::string f_get_time();
};

}



#endif //AUTO_SORTING_MACHINE_LOGGER_H
