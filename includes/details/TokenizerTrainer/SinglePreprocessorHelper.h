//
// Created by QinBu_Rua on 2026/1/31.
//

#ifndef AUTO_SORTING_MACHINE_SINGLEPREPROCESSOR_H
#define AUTO_SORTING_MACHINE_SINGLEPREPROCESSOR_H

#include <string>

#include "classes/CharTypeArray.h"



namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer {

class SinglePreprocessorHelper {
public:
   SinglePreprocessorHelper();
   SinglePreprocessorHelper(const std::wstring& sentence);

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

}



#endif //AUTO_SORTING_MACHINE_SINGLEPREPROCESSOR_H
