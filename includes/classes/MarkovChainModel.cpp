//
// Created by QinBu_Rua on 2026/1/17.
//

#include "MarkovChainModel.h"

using namespace QinBuRua::auto_sorting_machine;

double MarkovChainModel::get_ISD(CharType charType) const {
   return m_InitialStateDistribution[static_cast<size_t>(charType)];
}

double MarkovChainModel::get_TP(CharType from, CharType to) const {
   return m_TransitionProbability[static_cast<size_t>(from)][static_cast<size_t>(to)];
}

double MarkovChainModel::get_EP(const wchar_t wch, CharType tp) const {
   auto arr = m_EmissionProbability.find(wch);
   if (arr != m_EmissionProbability.end()) {
      return arr->second[static_cast<size_t>(tp)];
   } else {
      return 0.0;
   }
}

void MarkovChainModel::set_ISD(CharType charType, const double probability) {
   m_InitialStateDistribution[static_cast<size_t>(charType)] = probability;
}

void MarkovChainModel::set_TP(CharType from, CharType to, const double probability) {
   m_TransitionProbability[static_cast<size_t>(from)][static_cast<size_t>(to)] = probability;
}

void MarkovChainModel::set_EP(const wchar_t wch, CharType tp, const double probability) {
   m_EmissionProbability[static_cast<size_t>(wch)][static_cast<size_t>(tp)] = probability;
}

void MarkovChainModel::set_ISDs(
   const unsigned int singleTimes,
   const unsigned int beginTimes
) {
   const double allTimes = singleTimes + beginTimes;

   m_InitialStateDistribution[0] = singleTimes / allTimes;
   m_InitialStateDistribution[1] = beginTimes / allTimes;
}
