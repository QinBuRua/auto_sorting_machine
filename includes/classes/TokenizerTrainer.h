//
// Created by QinBu_Rua on 2025/12/27.
//

#ifndef AUTO_SORTING_MACHINE_TOKENIZER_H
#define AUTO_SORTING_MACHINE_TOKENIZER_H

#include <array>
#include <unordered_map>
#include <utility>

#include "json.hpp"

#include "CharTypeArray.h"
#include "MarkovChainModel.h"


namespace QinBuRua::auto_sorting_machine {

class TokenizerTrainer {
public:
   TokenizerTrainer() = default;
   TokenizerTrainer(const nlohmann::json& config_json);

   void destroy();
   void load_config(const nlohmann::json& config_json);
   void run();

private:
   nlohmann::json m_Config;
   std::vector<std::wstring> m_Sentences;
   std::vector<CharTypeArray> m_CharTypeArrays;

   MarkovChainModel m_MarkovModel;

private:
   void f_read_files(const std::string& path, const std::string& code = "utf-8");

   void f_initialize();
   void f_preprocess();
   void f_train_ISD();
   void f_train_TP();
   void f_train_EP();


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
      [[nodiscard]] wchar_t f_peek_char() const;
      void f_parse_word();
   };


   class c_EPTrainer {
   public:
      c_EPTrainer();

      void load(
         const std::vector<std::wstring>& sentences,
         const std::vector<CharTypeArray>& char_type_arrays,
         MarkovChainModel& markov_chain
      );
      void run();

   private:
      std::span<const std::wstring> m_Sentences;
      std::span<const CharTypeArray> m_CharTypeArrays;
      MarkovChainModel* m_MarkovModel;

      std::array<size_t, 2> m_ChIndex;
      std::array<size_t, 2> m_ChtIndex;
      std::pair<wchar_t, CharType> m_Ch;

      std::unordered_map<wchar_t, std::array<unsigned int, 4>> m_CharToTypeTimes;

   private:
      bool f_read_token_char(); //true为正常，false为结束
   };
};

}

#endif //AUTO_SORTING_MACHINE_TOKENIZER_H
