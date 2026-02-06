//
// Created by QinBu_Rua on 2025/12/27.
//

#ifndef AUTO_SORTING_MACHINE_TOKENIZER_H
#define AUTO_SORTING_MACHINE_TOKENIZER_H

#include <source_location>
#include <unordered_map>

#include "json.hpp"

#include "CharTypeArray.h"
#include "MarkovChainModel.h"


namespace QinBuRua::auto_sorting_machine {

class TokenizerTrainer {
public:
   TokenizerTrainer() = default;
   explicit TokenizerTrainer(const nlohmann::json& config_json);

   void destroy();
   void load_config(const nlohmann::json& config_json);
   void run();

   ModelHeader& header();
   MarkovChainModel& get_model_ref();

   std::vector<uint8_t> get_model_data(); //包括模型头
   void write_to_file(const std::string& filename, const std::source_location& sl = std::source_location::current()) const;

private:
   nlohmann::json m_Config;
   std::vector<std::wstring> m_Sentences;
   std::vector<CharTypeArray> m_CharTypeArrays;

   bool m_IsDone = false;
   MarkovChainModel m_MarkovModel;
   std::vector<uint8_t> m_RawModelData{};
   std::vector<uint8_t> m_RawHeaderData{};

private:
   void f_read_files(const std::string& path, const std::string& code = "utf-8");

   void f_initialize();
   void f_preprocess();
   void f_train_isd();
   void f_train_tp();
   void f_train_ep();
   void f_done();
};

}

#endif //AUTO_SORTING_MACHINE_TOKENIZER_H
