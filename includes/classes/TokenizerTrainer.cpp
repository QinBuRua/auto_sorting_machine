//
// Created by QinBu_Rua on 2025/12/27.
//

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include "TokenizerTrainer.h"

#include <iostream>

using std::vector;
using std::string;
using std::wstring;
using std::wifstream;
using std::runtime_error;

using nlohmann::json;

namespace fs = std::filesystem;
using namespace QinBuRua::auto_sorting_machine;

TokenizerTrainer::TokenizerTrainer(const json& configJson) {
   m_Config = configJson;
}

void TokenizerTrainer::destroy() {
   m_Config.clear();
   m_Sentences.clear();
   m_CharTypeArrays.clear();
}

void TokenizerTrainer::load_config(const nlohmann::json& configJson) {
   destroy();
   m_Config = configJson;
}

void TokenizerTrainer::run() {
   f_initialize();
   f_preprocess();
}

void TokenizerTrainer::f_read_files(const string& path, const string& code) {
   if (!(fs::exists(path) && fs::is_directory(path))) {
      throw std::runtime_error("The direction of training files does NOT exist!");
   }
   fs::path trainingFilesPath(path);
   vector<fs::path> files;
   for (const auto& entry: fs::directory_iterator(trainingFilesPath)) {
      if (entry.is_regular_file() && entry.path().extension() == ".txt") {
         files.push_back(entry.path());
      }
   }
   if (files.empty()) {
      throw runtime_error("No training files found!");
   }
   std::locale localeCode;
   if (code == "utf-8") {
      localeCode = std::locale(std::locale(), new std::codecvt_utf8<wchar_t>);
   }
   for (const auto& file: files) {
      wifstream fin(file);
      if (fin.fail()) {
         throw runtime_error("Error opening file \"!" + file.string() + "\"!");
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
   f_read_files(m_Config["direction"],
      m_Config["code"].is_null() ? "utf-8" : m_Config["code"]);
   if (m_Sentences.empty()) {
      throw runtime_error("No training sentences found!");
   }
}

void TokenizerTrainer::f_preprocess() {
   c_SinglePreprocessor preprocessor;
   for (const auto& sentence: m_Sentences) {
      preprocessor.load(sentence);
      preprocessor.run();
      m_CharTypeArrays.push_back(
         std::move(preprocessor.get_result_ref())
      );
   }
}

TokenizerTrainer::c_SinglePreprocessor::c_SinglePreprocessor() {
   m_Sentence = nullptr;
   m_Index    = 0;
   m_Char     = '\0';
}

TokenizerTrainer::c_SinglePreprocessor::c_SinglePreprocessor(const std::wstring& sentence) {
   m_Sentence = &sentence;
   m_Index    = 0;
   while (m_Index < m_Sentence->size()) {
      if (!std::iswspace((*m_Sentence)[m_Index])) {
         m_Char = (*m_Sentence)[m_Index];
         break;
      }
      m_Index++;
   }
}

void TokenizerTrainer::c_SinglePreprocessor::clear() {
   m_CharTypes.destroy();
}

void TokenizerTrainer::c_SinglePreprocessor::load(const std::wstring& sentence) {
   clear();
   m_Sentence = &sentence;
   m_Index    = 0;
   while (m_Index < m_Sentence->size()) {
      if (!std::iswspace((*m_Sentence)[m_Index])) {
         m_Char = (*m_Sentence)[m_Index];
         break;
      }
      m_Index++;
   }
}

void TokenizerTrainer::c_SinglePreprocessor::run() {
   while (m_Index < m_Sentence->size()) {
      f_parse_word();
      f_read_token_char();
   }
}

CharTypeArray& TokenizerTrainer::c_SinglePreprocessor::get_result_ref() {
   return m_CharTypes;
}

wchar_t TokenizerTrainer::c_SinglePreprocessor::f_read_token_char() {
   m_Index++;
   while (m_Index < m_Sentence->size()) {
      if (!std::iswspace((*m_Sentence)[m_Index])) {
         m_Char = (*m_Sentence)[m_Index];
         break;
      }
      m_Index++;
   }
   return m_Char;
}

wchar_t TokenizerTrainer::c_SinglePreprocessor::f_peek_char() const {
   return (*m_Sentence)[m_Index + 1];
}

void TokenizerTrainer::c_SinglePreprocessor::f_parse_word() {
   if (m_Index + 1 == m_Sentence->size() || std::iswspace(f_peek_char())) {
      m_CharTypes.push_back(CharType::SINGLE);
      return;
   }
   m_CharTypes.push_back(CharType::BEGIN);
   do {
      f_read_token_char();
      m_CharTypes.push_back(CharType::MIDDLE);
   } while (m_Index + 1 < m_Sentence->size() && !std::iswspace(f_peek_char()));
   m_CharTypes.set_back(CharType::END);
}
