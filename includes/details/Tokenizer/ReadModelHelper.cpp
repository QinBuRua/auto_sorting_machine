//
// Created by QinBu_Rua on 2026/2/7.
//

#include <filesystem>

#include "Logger/Logger.h"

#include "ReadModelHelper.h"

namespace fs=std::filesystem;
using namespace QinBuRua::auto_sorting_machine::details::tokenizer;
namespace slog = QinBuRua::auto_sorting_machine::utils::log;

ReadModelHelper::ReadModelHelper(const std::string& filename) {
   set_model_file(filename);
}

void ReadModelHelper::clear() {
   m_FileName.clear();
   m_RawData.clear();
   m_MarkovModel.clear();
}
