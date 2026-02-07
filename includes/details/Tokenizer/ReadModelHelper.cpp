//
// Created by QinBu_Rua on 2026/2/7.
//

#include <format>

#include "Logger/Logger.h"

#include "ReadModelHelper.h"

using namespace QinBuRua::auto_sorting_machine::details::tokenizer;
namespace slog = QinBuRua::auto_sorting_machine::utils::log;

ReadModelHelper::ReadModelHelper(const std::string& filename) {
   set_model_file(filename);
}

void ReadModelHelper::clear() {
   m_FileName.clear();
   m_Fin.close();
   m_RawData.clear();
   m_MarkovModel.clear();
}

void ReadModelHelper::set_model_file(const std::string& filename) {
   clear();
   m_FileName = filename;
   m_Fin.open(m_FileName, std::ios::binary | std::ios::ate);
   if (m_Fin.fail()) {
      slog::error_throw<std::runtime_error>(
         slog::Tag{},
         std::format("Fail to open file \"{}\"", filename)
      );
   }
   m_FileSize = m_Fin.tellg();
   m_Fin.seekg(0, std::ios::beg);
}

void ReadModelHelper::run() {
   f_initialize();
}

void ReadModelHelper::f_initialize() {
   m_Fin.seekg(0, std::ios::end);

}
