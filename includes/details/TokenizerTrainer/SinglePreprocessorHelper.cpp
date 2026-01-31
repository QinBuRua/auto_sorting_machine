//
// Created by QinBu_Rua on 2026/1/31.
//

#include "SinglePreprocessorHelper.h"

using QinBuRua::auto_sorting_machine::CharType;
using QinBuRua::auto_sorting_machine::CharTypeArray;
using namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer;

SinglePreprocessorHelper::SinglePreprocessorHelper() {
   m_Sentence = nullptr;
   m_Index    = 0;
   m_Char     = '\0';
}

SinglePreprocessorHelper::SinglePreprocessorHelper(const std::wstring& sentence) {
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

void SinglePreprocessorHelper::clear() {
   m_CharTypes.destroy();
}

void SinglePreprocessorHelper::load(const std::wstring& sentence) {
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

void SinglePreprocessorHelper::run() {
   while (m_Index < m_Sentence->size()) {
      f_parse_word();
      f_read_token_char();
   }
}

CharTypeArray& SinglePreprocessorHelper::get_result_ref() {
   return m_CharTypes;
}

wchar_t SinglePreprocessorHelper::f_read_token_char() {
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

wchar_t SinglePreprocessorHelper::f_peek_char() const {
   return (*m_Sentence)[m_Index + 1];
}

void SinglePreprocessorHelper::f_parse_word() {
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
