//
// Created by QinBu_Rua on 2026/2/10.
//

#include <filesystem>
#include <format>
#include <ranges>
#include <string>

#include "ReadFilesHelper.h"

#include <algorithm>

#include "Logger/Logger.h"
#include "utfcpp/utf8.h"

namespace fs = std::filesystem;
namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer;
namespace slog = QinBuRua::auto_sorting_machine::utils::log;

ReadFilesHelper::ReadFilesHelper(const std::string& path) {
   load(path);
}

void ReadFilesHelper::clear() {
   m_Path.clear();
   m_Files.clear();
   m_TrainData.clear();
}

void ReadFilesHelper::load(const std::string& path, const std::source_location& sl) {
   clear();
   m_Path = path;
}

void ReadFilesHelper::run(const std::source_location& sl) {
   f_initialize(sl);
   f_read_files(sl);
}

std::vector<std::u16string>& ReadFilesHelper::get_data_ref() {
   return m_TrainData;
}

void ReadFilesHelper::f_read_file(const std::filesystem::path& file_path, const std::source_location& sl) {
   std::ifstream file_stream{file_path};
   if (file_stream.fail()) {
      slog::warn_sl(std::format("Fail to open file \"{}\"", file_path.string()), sl);
      slog::info_sl(std::format("Close file \"{}\"", file_path.string()), sl);
      return;
   }

   uint32_t line_number = 0;
   std::string line;
   while (std::getline(file_stream, line)) {
      ++line_number;
      if (line.empty()) {
         continue;
      }
      if (utf8::is_valid(line)) {
         m_TrainData.push_back(std::move(utf8::utf8to16(line)));
      } else {
         slog::warn_sl(
            std::format("The code of line {} in file \"{}\"  is NOT utf-8", line_number, file_path.string()), sl
         );
      }
   }
   if (file_stream.eof()) {
      slog::info_sl(std::format("Complete reading file \"{}\"", file_path.string()));
   } else if (file_stream.fail()) {
      slog::warn_sl(std::format("Fail to read line {} in file \"{}\"", line_number, file_path.string()), sl);
      slog::info_sl(std::format("Close file \"{}\"", file_path.string()));
   } else {
      slog::error_throw_sl<std::runtime_error>(
         slog::Tag{sl},
         std::format(
            "Unknow Error while reading line {} in file \"{}\"",
            line_number, file_path.string()
         )
      );
   }
}

void ReadFilesHelper::f_initialize(const std::source_location& sl) {
   if (!fs::exists(m_Path) || !fs::is_directory(m_Path)) {
      slog::error_throw_sl<std::runtime_error>(slog::Tag{sl}, "The direction of training files does NOT exist!");
   }
   fs::path path{m_Path};
   for (const auto& entry: fs::directory_iterator(path)) {
      if (entry.is_regular_file() && entry.path().extension() == ".txt") {
         m_Files.push_back(entry.path());
      }
   }
   if (m_Files.empty()) {
      slog::error_throw_sl<std::runtime_error>(slog::Tag{sl}, "No training files found!");
   }
}

void ReadFilesHelper::f_read_files(const std::source_location& sl) {
   ranges::for_each(
      m_Files, [this,&sl](const auto& file_path) {
         f_read_file(file_path, sl);
      }
   );
}
