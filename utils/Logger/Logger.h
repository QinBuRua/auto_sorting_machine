//
// Created by QinBu_Rua on 2026/2/6.
//

#ifndef AUTO_SORTING_MACHINE_LOGGER_H
#define AUTO_SORTING_MACHINE_LOGGER_H

#include <cstdint>
#include <fstream>
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
   static Logger& instance();

   void set_log_file(
      const std::string& filename,
      const std::source_location& sl = std::source_location::current()
   );
   void set_log_level(LogLevel level) noexcept; //只有debug等级会包含源码信息

   std::string get_log_file() const;
   LogLevel get_log_level() const;

   void auto_initialize(
      LogLevel level                 = LogLevel::INFO,
      const std::source_location& sl = std::source_location::current()
   );

   std::string log(
      LogLevel level,
      const std::string& message,
      const std::source_location& sl = std::source_location::current()
   );

   Logger(const Logger&)            = delete;
   Logger(Logger&&)                 = delete;
   Logger& operator=(const Logger&) = delete;
   Logger& operator=(Logger&&)      = delete;

private:
   std::string m_FileName;
   std::ofstream m_Fout;
   LogLevel m_LogLevel;

private:
   Logger();

   std::string f_make_message(LogLevel level, const std::string& message, const std::source_location& sl);
   static std::string f_get_time_str();
};

namespace log {
//已经封装好的，直接用，不需要初始化

struct Tag {
   LogLevel level;
   std::source_location location;
   explicit Tag(const std::source_location& sl = std::source_location::current());
   explicit Tag(LogLevel lvl, const std::source_location& sl = std::source_location::current());

   Tag(Tag&&)                 = delete;
   Tag(const Tag&)            = delete;
   Tag& operator=(Tag&&)      = delete;
   Tag& operator=(const Tag&) = delete;
};

void debug(const std::string& message, const std::source_location& sl = std::source_location::current());
void info(const std::string& message, const std::source_location& sl = std::source_location::current());
void warn(const std::string& message, const std::source_location& sl = std::source_location::current());
void error(const std::string& message, const std::source_location& sl = std::source_location::current());
void fatal(const std::string& message, const std::source_location& sl = std::source_location::current());
template<typename T, typename... Args>
void log_throw(Tag tag, Args&&... args);
template<typename T, typename... Args>
void debug_throw(Tag tag, Args&&... args);
template<typename T, typename... Args>
void info_throw(Tag tag, Args&&... args);
template<typename T, typename... Args>
void warn_throw(Tag tag, Args&&... args);
template<typename T, typename... Args>
void error_throw(Tag tag, Args&&... args);
template<typename T, typename... Args>
void fatal_throw(Tag tag, Args&&... args);


}

#include "Logger.inl"

}



#endif //AUTO_SORTING_MACHINE_LOGGER_H
