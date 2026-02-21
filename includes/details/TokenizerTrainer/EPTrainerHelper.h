//
// Created by QinBu_Rua on 2026/1/31.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZERTRAINER_EPTRAINERHELPER_H
#define AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZERTRAINER_EPTRAINERHELPER_H

#include <span>
#include <string>
#include <vector>

#include "classes/CharTypeArray.h"
#include "classes/MarkovChainModel.h"


namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer {

class EpTrainerHelper {
public:
   EpTrainerHelper();

   void load(
      const std::vector<std::u16string>& sentences,
      const std::vector<CharTypeArray>& char_type_arrays,
      MarkovChainModel& markov_chain
   );
   void run();

private:
   std::span<const std::u16string> m_Sentences;
   std::span<const CharTypeArray> m_CharTypeArrays;
   MarkovChainModel* m_MarkovModel;

   std::array<size_t, 2> m_ChIndex;
   std::array<size_t, 2> m_ChtIndex;
   std::pair<char16_t, CharType> m_Ch;

   std::unordered_map<char16_t, std::array<unsigned int, 4>> m_CharToTypeTimes;

private:
   bool f_read_token_char(); //true为正常，false为结束
};

}



#endif //AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZERTRAINER_EPTRAINERHELPER_H
