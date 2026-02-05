//
// Created by QinBu_Rua on 2026/1/31.
//

#include <numeric>
#include <utility>

#include "EPTrainerHelper.h"


using namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer;

EPTrainerHelper::EPTrainerHelper() : m_MarkovModel(nullptr),
                                     m_ChIndex({}),
                                     m_ChtIndex({}) {
}

void EPTrainerHelper::load(
   const std::vector<std::wstring>& sentences,
   const std::vector<CharTypeArray>& char_type_arrays,
   MarkovChainModel& markov_chain
) {
   m_Sentences      = sentences;
   m_CharTypeArrays = char_type_arrays;
   m_MarkovModel    = &markov_chain;
   m_ChIndex.fill(0);
   m_ChtIndex.fill(0);
   m_Ch = std::make_pair(
      m_Sentences[m_ChIndex[0]][m_ChIndex[1]],
      m_CharTypeArrays[m_ChtIndex[0]].get(m_ChtIndex[1])
   );
}

void EPTrainerHelper::run() {
   do {
      m_CharToTypeTimes[m_Ch.first][std::to_underlying(m_Ch.second)]++;
   } while (f_read_token_char());

   for (const auto& [wch, counts]: m_CharToTypeTimes) {
      const double allTimes = std::accumulate<decltype(counts.begin()), unsigned int>
         (counts.begin(), counts.end(), 0);

      m_MarkovModel->set_ep(
         wch,
         CharType::SINGLE,
         counts[std::to_underlying(CharType::SINGLE)] / allTimes
      );
      m_MarkovModel->set_ep(
         wch,
         CharType::BEGIN,
         counts[std::to_underlying(CharType::BEGIN)] / allTimes
      );
      m_MarkovModel->set_ep(
         wch,
         CharType::MIDDLE,
         counts[std::to_underlying(CharType::MIDDLE)] / allTimes
      );
      m_MarkovModel->set_ep(
         wch,
         CharType::END,
         counts[std::to_underlying(CharType::END)] / allTimes
      );
   }
}

bool EPTrainerHelper::f_read_token_char() {
   do {
      m_ChIndex[1]++;
   } while (m_Sentences[m_ChIndex[0]][m_ChIndex[1]] == L' '
      && m_ChIndex[1] < m_Sentences[m_ChIndex[0]].size());
   if (m_ChIndex[1] >= m_Sentences[m_ChIndex[0]].size()) {
      m_ChIndex[0]++;
      if (m_ChIndex[0] >= m_Sentences.size()) {
         return false;
      }
      m_ChtIndex[0]++;
      m_ChIndex[1]  = 0;
      m_ChtIndex[1] = 0;
   } else {
      m_ChtIndex[1]++;
   }
   m_Ch = std::make_pair(
      m_Sentences[m_ChIndex[0]][m_ChIndex[1]],
      m_CharTypeArrays[m_ChtIndex[0]].get(m_ChtIndex[1])
   );
   return true;
}
