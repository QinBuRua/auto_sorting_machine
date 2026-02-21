//
// Created by QinBu_Rua on 2025/12/27.
//

#include <array>
#include <filesystem>
#include <fstream>
#include <numeric>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "TokenizerTrainer.h"

#include "details/TokenizerTrainer/EPTrainerHelper.h"
#include "details/TokenizerTrainer/ReadFilesHelper.h"
#include "details/TokenizerTrainer/SinglePreprocessorHelper.h"
#include "Logger/Logger.h"

using nlohmann::json;

namespace fs = std::filesystem;
namespace stdr = std::ranges;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;
using details::tokenizer_trainer::EPTrainerHelper;
using details::tokenizer_trainer::ReadFilesHelper;
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
   if (m_IsDone) {
      slog::warn_throw_sl<std::runtime_error>(
         slog::Tag{},
         "You can't train this model again!"
      );
   }
   f_initialize();
   f_preprocess();
   f_train_isd();
   f_train_tp();
   f_train_ep();
   f_done();
}

MarkovChainModel& TokenizerTrainer::get_model_ref() {
   return m_MarkovModel;
}

std::vector<std::byte> TokenizerTrainer::get_model_data() {
   std::vector<std::byte> result(m_RawModelData.size() + m_RawHeaderData.size());
   auto iter = result.begin();
   iter      = stdr::copy(m_RawHeaderData, iter).out;
   stdr::copy(m_RawModelData, iter);
   return result;
}

void TokenizerTrainer::write_to_file(const std::string& filename, const std::source_location& sl) const {
   std::ofstream fout(filename, std::ios::binary);
   if (fout.fail()) {
      slog::error_throw_sl<std::runtime_error>(
         slog::Tag{sl},
         std::format(
            "Fail to open file \"{}\"",
            filename
         )
      );
   }
   fout.write(reinterpret_cast<const char*>(m_RawHeaderData.data()), m_RawHeaderData.size());
   fout.write(reinterpret_cast<const char*>(m_RawModelData.data()), m_RawModelData.size());
   if (fout.fail()) {
      slog::error_throw_sl<std::runtime_error>(
         slog::Tag{sl},
         std::format(
            "Fail to write file \"{}\"",
            filename
         )
      );

   }
}

void TokenizerTrainer::f_read_files(const std::string& path, const std::string& code) {
   ReadFilesHelper helper;
   if (code != "utf-8") {
      slog::error_throw_sl<std::runtime_error>(slog::Tag{}, "Only support utf-8");
   }
   helper.load(path);
   helper.run();
   m_Sentences = std::move(helper.get_data_ref());
}

void TokenizerTrainer::f_initialize() {
   if (m_Config["direction"].is_null()) {
      slog::error_throw_sl<std::runtime_error>(slog::Tag{}, "Training config is MISSING!");
   }
   f_read_files(
      m_Config["direction"],
      m_Config["code"].is_null() ? "utf-8" : m_Config["code"]
   );
   if (m_Sentences.empty()) {
      slog::error_throw_sl<std::runtime_error>(slog::Tag{}, "No training sentences found!");
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

void TokenizerTrainer::f_done() {
   m_CharTypeArrays.clear();
   m_Sentences.clear();
   m_Config.clear();

   m_RawModelData = std::move(m_MarkovModel.get_binary_data());
   m_MarkovModel.header().set_sha256_from(
      reinterpret_cast<const unsigned char*>(m_RawModelData.data()),
      reinterpret_cast<const unsigned char*>(m_RawModelData.data() + m_RawModelData.size())
   );
   m_RawHeaderData = std::move(m_MarkovModel.header().get_binary_model_data());
}
