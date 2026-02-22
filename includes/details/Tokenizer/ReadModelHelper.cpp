//
// Created by QinBu_Rua on 2026/2/7.
//

#include <filesystem>
#include <fstream>

#include "ReadModelHelper.h"
#include "details/MarkovChainModel/ParseModelHelper.h"
#include "details/ModelHeader/ParseHeaderHelper.h"
#include "Logger/Logger.h"

namespace stdf = std::filesystem;
using namespace QinBuRua::auto_sorting_machine;
using namespace details::tokenizer;
namespace slog = utils::log;

ReadModelHelper::ReadModelHelper(const std::string& filename) {
   set_model_file(filename);
}

void ReadModelHelper::clear() {
   m_FileName.clear();
   m_RawData.clear();
   m_Header.clear();
   m_MarkovModel.clear();
}

void ReadModelHelper::set_model_file(const std::string& filename) {
   if (!stdf::is_regular_file(filename)) {
      slog::error_throw<std::runtime_error>(std::format("File \"{}\" NOT exists!", filename));
   }
   m_FileName = filename;
}

void ReadModelHelper::run() {
   f_initialize();
   f_parse_header();
   f_parse_model();
   f_complete();
}

MarkovChainModel& ReadModelHelper::get_model_ref() {
   return m_MarkovModel;
}

void ReadModelHelper::f_initialize() {
   m_RawData.clear();
   m_MarkovModel.clear();

   const auto filePath = stdf::path(m_FileName);
   const auto fileSize = stdf::file_size(filePath);
   std::ifstream fileStream{m_FileName, std::ios::binary};
   if (fileStream.fail()) {
      slog::error_throw<std::runtime_error>(std::format("Fail to open model file \"{}\"", filePath.string()));
   }

   m_RawData.resize(fileSize);
   fileStream.read(reinterpret_cast<char*>(m_RawData.data()), fileSize);
   if (fileStream.fail()) {
      slog::error_throw<std::runtime_error>(std::format("Fail to read model file \"{}\"", filePath.string()));
   }

   m_Iterator = m_RawData.begin();
}

void ReadModelHelper::f_parse_header() {
   model_header::ParseHeaderHelper headerParser{m_Iterator};
   headerParser.run();
   m_Header   = std::move(headerParser.get_header_ref());
   m_Iterator = headerParser.get_iter();
}

void ReadModelHelper::f_parse_model() {
   markov_chain_model::ParseModelHelper modelParser{m_Iterator};
   modelParser.run();
   m_MarkovModel          = std::move(modelParser.get_model_ref());
   m_MarkovModel.header() = std::move(m_Header);
}

void ReadModelHelper::f_complete() {
   m_RawData.clear();
   m_MarkovModel.clear();
}
