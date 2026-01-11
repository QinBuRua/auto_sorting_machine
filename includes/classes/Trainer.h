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
   explicit Trainer(const std::string& configFile);
   explicit Trainer(const nlohmann::json& configJson);

   void run();

private:
   nlohmann::json m_Config;
   TokenizerTrainer m_Tokenizer;


};

}

#endif //AUTO_SORTING_MACHINE_TRAINER_H
