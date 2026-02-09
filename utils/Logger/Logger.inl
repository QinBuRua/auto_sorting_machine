#ifndef AUTO_SORTING_MACHINE_LOGGER_INL
#define AUTO_SORTING_MACHINE_LOGGER_INL

template<typename T, typename... Args>
void log::log_throw(Tag tag, Args&&... args) {
   static_assert(
      std::is_base_of_v<std::exception, T>,
      "T must base on std::exception"
   );
   T exc{std::forward<Args>(args)...};
   Logger::instance().log(tag.level, exc.what(), tag.location);
   throw exc;
}

template<typename T, typename... Args>
void log::debug_throw(Tag tag, Args&&... args) {
   log_throw<T>(Tag{LogLevel::DEBUG, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::info_throw(Tag tag, Args&&... args) {
   log_throw<T>(Tag{LogLevel::INFO, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename ... Args>
void log::warn_throw(Tag tag, Args&&... args) {
   log_throw<T>(Tag{LogLevel::WARN, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::error_throw(Tag tag, Args&&... args) {
   log_throw<T>(Tag{LogLevel::ERROR, tag.location}, std::forward<Args>(args)...);
}

template<typename T, typename... Args>
void log::fatal_throw(Tag tag, Args&&... args) {
   log_throw<T>(Tag{LogLevel::FATAL, tag.location}, std::forward<Args>(args)...);
}

#endif // AUTO_SORTING_MACHINE_LOGGER_INL
