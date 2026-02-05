//
// Created by QinBu_Rua on 2025/12/27.
//

#include <array>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "TokenizerTrainer.h"
#include "details/TokenizerTrainer/EPTrainerHelper.h"
#include "details/TokenizerTrainer/SinglePreprocessorHelper.h"

using nlohmann::json;

namespace fs = std::filesystem;
using namespace QinBuRua::auto_sorting_machine;
using details::tokenizer_trainer::EPTrainerHelper;
using details::tokenizer_trainer::SinglePreprocessorHelper;

TokenizerTrainer::TokenizerTrainer(const json& config_json) {
   m_Config = config_json;
}

void TokenizerTrainer::destroy() {
   m_Config.clear();
   m_Sentences.clear();
   m_CharTypeArrays.clear();
}

void TokenizerTrainer::load_config(const nlohmann::json& config_json) {
   destroy();
   m_Config = config_json;
}

ModelHeader& TokenizerTrainer::header() {
   return m_MarkovModel.header();
}

void TokenizerTrainer::run() {
   f_initialize();
   f_preprocess();
   f_train_isd();
   f_train_tp();
   f_train_ep();
}

MarkovChainModel& TokenizerTrainer::get_model_ref() {
   return m_MarkovModel;
}

void TokenizerTrainer::f_read_files(const std::string& path, const std::string& code) {
   if (!(fs::exists(path) && fs::is_directory(path))) {
      throw std::runtime_error("The direction of training files does NOT exist!");
   }
   fs::path trainingFilesPath(path);
   std::vector<fs::path> files;
   for (const auto& entry: fs::directory_iterator(trainingFilesPath)) {
      if (entry.is_regular_file() && entry.path().extension() == ".txt") {
         files.push_back(entry.path());
      }
   }
   if (files.empty()) {
      throw std::runtime_error("No training files found!");
   }
   std::locale localeCode;
   if (code == "utf-8") {
      localeCode = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>);
   }
   for (const auto& file: files) {
      std::wifstream fin(file);
      if (fin.fail()) {
         throw std::runtime_error("Error opening file \"!" + file.string() + "\"!");
      }
      fin.imbue(localeCode);
      do {
         m_Sentences.emplace_back();
      } while (std::getline(fin, m_Sentences.back()));
      if (m_Sentences.back().empty()) {
         m_Sentences.pop_back();
      }
   }
}

void TokenizerTrainer::f_initialize() {
   if (m_Config["direction"].is_null()) {
      throw std::runtime_error("Training config is MISSING!");
   }
   f_read_files(
      m_Config["direction"],
      m_Config["code"].is_null() ? "utf-8" : m_Config["code"]
   );
   if (m_Sentences.empty()) {
      throw std::runtime_error("No training sentences found!");
   }
}

void TokenizerTrainer::f_preprocess() {
   SinglePreprocessorHelper preprocessor;
   for (const auto& sentence: m_Sentences) {
      preprocessor.load(sentence);
      preprocessor.run();
      m_CharTypeArrays.push_back(
         std::move(preprocessor.get_result_ref())
      );
   }
}

void TokenizerTrainer::f_train_isd() {
   std::array<unsigned int, 2> counts{};
   for (auto& ct: m_CharTypeArrays) {
      counts[std::to_underlying(ct.get_front())]++;
   }
   m_MarkovModel.set_isd_s(
      counts[0],
      counts[1]
   );
}

void TokenizerTrainer::f_train_tp() {
   std::array<std::array<unsigned int, 4>, 4> counts{};
   for (const auto& iter: m_CharTypeArrays) {
      if (iter.size() == 1 || iter.empty())
         continue;
      for (size_t i = 1; i < iter.size(); ++i) {
         ++counts[std::to_underlying(iter.get(i - 1))][std::to_underlying(iter.get(i))];
      }
   }
   for (int i = 0; i < 4; i++) {
      const double sum = std::accumulate(
         counts[i].begin(),
         counts[i].end(),
         static_cast<unsigned int>(0)
      );
      for (int j = 0; j < 4; j++) {
         m_MarkovModel.set_tp(
            static_cast<CharType>(i),
            static_cast<CharType>(j),
            counts[i][j] / sum
         );
      }
   }
}

void TokenizerTrainer::f_train_ep() {
   EPTrainerHelper epTrainer;
   epTrainer.load(m_Sentences, m_CharTypeArrays, m_MarkovModel);
   epTrainer.run();
}

