//
// Created by QinBu_Rua on 2025/12/27.
//

#ifndef AUTO_SORTING_MACHINE_TRAINER_H
#define AUTO_SORTING_MACHINE_TRAINER_H

#include <iostream>

#include "json.hpp"

#include "TokenizerTrainer.h"

namespace QinBuRua::auto_sorting_machine {

class Trainer {
public:
   Trainer() = default;
   explicit Trainer(const std::string& config_file);
   explicit Trainer(const nlohmann::json& config_json);

   void clear();
   void load_config(const std::string& config_file);
   void load_config(const nlohmann::json& config_json);
   void load_config(const char* config_file_chptr);
   void run();

   TokenizerTrainer& get_tokenizer_trainer();

private:
   nlohmann::json m_Config;
   TokenizerTrainer m_Tokenizer;

};

}

#endif //AUTO_SORTING_MACHINE_TRAINER_H
