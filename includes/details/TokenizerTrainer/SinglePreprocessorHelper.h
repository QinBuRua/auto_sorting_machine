//
// Created by QinBu_Rua on 2026/1/31.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZERTRAINER_SINGLEPREPROCESSOR_H
#define AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZERTRAINER_SINGLEPREPROCESSOR_H

#include <string>

#include "classes/CharTypeArray.h"

namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer {

class SinglePreprocessorHelper {
public:
   SinglePreprocessorHelper();
   SinglePreprocessorHelper(const std::u16string& sentence);

   void clear();
   void load(const std::u16string& sentence);
   void run();
   CharTypeArray& get_result_ref();

private:
   const std::u16string* m_Sentence;
   CharTypeArray m_CharTypes;
   size_t m_Index;
   char16_t m_Char;

   char16_t f_read_token_char();
   [[nodiscard]] char16_t f_peek_char() const;
   void f_parse_word();
};

}



#endif //AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZERTRAINER_SINGLEPREPROCESSOR_H
