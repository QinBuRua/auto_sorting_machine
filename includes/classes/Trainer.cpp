//
// Created by QinBu_Rua on 2025/12/27.
//

//stl标准库级
#include <fstream>
#include <stdexcept>
//开源依赖库级
#include "json.hpp"
//自定义头文件级
#include "Trainer.h"

using Json = nlohmann::json;

using namespace QinBuRua::auto_sorting_machine;



Trainer::Trainer(const std::string& config_file) {
   std::ifstream fin(config_file);
   if (fin.fail()) {
      throw std::runtime_error("Error opening config file");
   }
   m_Config = Json::parse(fin);
   m_Tokenizer.load_config(m_Config["train_files"]);
}

Trainer::Trainer(const Json& config_json) {
   m_Config = config_json;
}

void Trainer::clear() {
   m_Config.clear();
   m_Tokenizer.destroy();
}

void Trainer::load_config(const std::string& config_file) {
   clear();
   std::ifstream fin(config_file);
   if (fin.fail()) {
      throw std::runtime_error("Error opening config file");
   }
   m_Config = Json::parse(fin);
   m_Tokenizer.load_config(m_Config["train_files"]);
}

void Trainer::load_config(const nlohmann::json& config_json) {
   clear();
   m_Config = config_json;
   m_Tokenizer.load_config(m_Config["train_files"]);
}

void Trainer::load_config(const char* config_file_chptr) {
   clear();
   std::ifstream fin(config_file_chptr);
   if (fin.fail()) {
      throw std::runtime_error("Error opening config file");
   }
   m_Config = Json::parse(fin);
   m_Tokenizer.load_config(m_Config["train_files"]);
}

void Trainer::run() {
   m_Tokenizer.run();
}
