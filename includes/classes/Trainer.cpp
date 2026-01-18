//
// Created by QinBu_Rua on 2025/12/27.
//

#include <fstream>
#include <stdexcept>

#include "json.hpp"
#include "Trainer.h"

using Json = nlohmann::json;

using namespace QinBuRua::auto_sorting_machine;



Trainer::Trainer(const std::string& configFile) {
   std::ifstream fin(configFile);
   if (fin.fail()) {
      throw std::runtime_error("Error opening config file");
   }
   m_Config = Json::parse(fin);
   m_Tokenizer.load_config(m_Config["train_files"]);
}

Trainer::Trainer(const Json& configJson) {
   m_Config = configJson;
}

void Trainer::run() {
   m_Tokenizer.run();
}
