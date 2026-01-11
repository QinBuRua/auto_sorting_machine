//
// Created by QinBu_Rua on 2025/12/27.
//

#ifndef AUTO_SORTING_MACHINE_TOKENIZER_H
#define AUTO_SORTING_MACHINE_TOKENIZER_H

#include "json.hpp"
#include "CharTypeArray.h"

namespace QinBuRua::auto_sorting_machine {

class TokenizerTrainer {
public:
   TokenizerTrainer() = default;
   TokenizerTrainer(const nlohmann::json& configJson);

   void destroy();
   void load_config(const nlohmann::json& configJson);
   void run();

private:
   nlohmann::json m_Config;
   std::vector<std::wstring> m_Sentences;
   std::vector<CharTypeArray> m_CharTypeArrays;

private:
   void f_read_files(const std::string& path, const std::string& code = "utf-8");
   void f_initialize();
   void f_preprocess();

   class c_SinglePreprocessor {
   public:
      c_SinglePreprocessor();
      c_SinglePreprocessor(const std::wstring& sentence);

      void clear();
      void load(const std::wstring& sentence);
      void run();
      CharTypeArray& get_result_ref();

   private:
      const std::wstring* m_Sentence;
      CharTypeArray m_CharTypes;
      size_t m_Index;
      wchar_t m_Char;

      wchar_t f_read_token_char();
      wchar_t f_peek_char() const;
      void f_parse_word();
   };
};

}

#endif //AUTO_SORTING_MACHINE_TOKENIZER_H
