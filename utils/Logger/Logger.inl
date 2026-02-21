#ifndef AUTO_SORTING_MACHINE_UTILS_LOGGER_LOGGER_INL
#define AUTO_SORTING_MACHINE_UTILS_LOGGER_LOGGER_INL

template<typename T, typename... Args>
void log::log_throw_sl(Tag tag, Args&&... args) {
   static_assert(
      std::is_base_of_v<std::exception, T>,
      "T must base on std::exception"
   );
   throw [&]()-> T {
      T exception{std::forward<Args>(args)...};
      Logger::instance().log_sl(tag.level, exception.what(), tag.location);
      return exception;
   }();
}

template<typename T, typename... Args>
void log::debug_throw_sl(Tag tag, Args&&... args) {
   log_throw_sl<T>(Tag{LogLevel::DEBUG, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::info_throw_sl(Tag tag, Args&&... args) {
   log_throw_sl<T>(Tag{LogLevel::INFO, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::warn_throw_sl(Tag tag, Args&&... args) {
   log_throw_sl<T>(Tag{LogLevel::WARN, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::error_throw_sl(Tag tag, Args&&... args) {
   log_throw_sl<T>(Tag{LogLevel::ERROR, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::fatal_throw_sl(Tag tag, Args&&... args) {
   log_throw_sl<T>(Tag{LogLevel::FATAL, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::log_throw(LogLevel level, Args&&... args) {
   static_assert(
      std::is_base_of_v<std::exception, T>,
      "T must derive from std::exception"
   );
   throw [&]()-> T {
      T exc{std::forward<Args>(args)...};
      Logger::instance().log(level, exc.what());
      return exc;
   }();
}

template<typename T, typename... Args>
void log::debug_throw(Args&&... args) {
   log_throw<T>(LogLevel::DEBUG, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::info_throw(Args&&... args) {
   log_throw<T>(LogLevel::INFO, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::warn_throw(Args&&... args) {
   log_throw<T>(LogLevel::WARN, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::error_throw(Args&&... args) {
   log_throw<T>(LogLevel::ERROR, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::fatal_throw(Args&&... args) {
   log_throw<T>(LogLevel::FATAL, std::forward<Args>(args)...);
}

#endif // AUTO_SORTING_MACHINE_UTILS_LOGGER_LOGGER_INL
