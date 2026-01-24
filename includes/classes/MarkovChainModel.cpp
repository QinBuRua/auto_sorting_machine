//
// Created by QinBu_Rua on 2026/1/17.
//

#include <utility>

#include "MarkovChainModel.h"

using namespace QinBuRua::auto_sorting_machine;

double MarkovChainModel::get_ISD(CharType charType) const {
   return m_InitialStateDistribution[std::to_underlying(charType)];
}

double MarkovChainModel::get_TP(CharType from, CharType to) const {
   return m_TransitionProbability[std::to_underlying(from)][std::to_underlying(to)];
}

double MarkovChainModel::get_EP(const wchar_t wch, CharType tp) const {
   auto arr = m_EmissionProbability.find(wch);
   if (arr != m_EmissionProbability.end()) {
      return arr->second[std::to_underlying(tp)];
   } else {
      return 0.0;
   }
}

void MarkovChainModel::set_ISD(CharType charType, const double probability) {
   m_InitialStateDistribution[std::to_underlying(charType)] = probability;
}

void MarkovChainModel::set_TP(CharType from, CharType to, const double probability) {
   m_TransitionProbability[std::to_underlying(from)][std::to_underlying(to)] = probability;
}

void MarkovChainModel::set_EP(const wchar_t wch, CharType tp, const double probability) {
   m_EmissionProbability[wch][std::to_underlying(tp)] = probability;
}

void MarkovChainModel::set_ISDs(
   const unsigned int single_times,
   const unsigned int begin_times
) {
   const double allTimes = single_times + begin_times;

   m_InitialStateDistribution[0] = single_times / allTimes;
   m_InitialStateDistribution[1] = begin_times / allTimes;
}
