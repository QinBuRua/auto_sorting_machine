//
// Created by QinBu_Rua on 2026/1/31.
//

#ifndef AUTO_SORTING_MACHINE_EPTRAINERHELPER_H
#define AUTO_SORTING_MACHINE_EPTRAINERHELPER_H

#include <span>
#include <string>
#include <vector>

#include "classes/CharTypeArray.h"
#include "classes/MarkovChainModel.h"


namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer {

class EPTrainerHelper {
public:
   EPTrainerHelper();

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

}



#endif //AUTO_SORTING_MACHINE_EPTRAINERHELPER_H
