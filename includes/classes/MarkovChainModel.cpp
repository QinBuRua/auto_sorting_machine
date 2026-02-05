//
// Created by QinBu_Rua on 2026/1/17.
//

#include <algorithm>
#include <utility>

#include "MarkovChainModel.h"
#include "details/MarkovChainModel/BinaryModelHelper.h"

using namespace QinBuRua::auto_sorting_machine;
using namespace details::markov_chain_model;

bool MarkovChainModel::has_ep(wchar_t charType) const {
   return m_EmissionProbability.contains(charType);
}

double MarkovChainModel::get_isd(CharType charType) const {
   return m_InitialStateDistribution[std::to_underlying(charType)];
}

double MarkovChainModel::get_tp(CharType from, CharType to) const {
   return m_TransitionProbability[std::to_underlying(from)][std::to_underlying(to)];
}

double MarkovChainModel::get_ep(const wchar_t wch, CharType tp) const {
   auto arr = m_EmissionProbability.find(wch);
   if (arr != m_EmissionProbability.end()) {
      return arr->second[std::to_underlying(tp)];
   } else {
      return 0.0;
   }
}

void MarkovChainModel::set_isd(CharType charType, const double probability) {
   m_InitialStateDistribution[std::to_underlying(charType)] = probability;
}

void MarkovChainModel::set_tp(CharType from, CharType to, const double probability) {
   m_TransitionProbability[std::to_underlying(from)][std::to_underlying(to)] = probability;
}

void MarkovChainModel::set_ep(const wchar_t wch, CharType tp, const double probability) {
   m_EmissionProbability[wch][std::to_underlying(tp)] = probability;
}

void MarkovChainModel::set_isd_s(
   const unsigned int single_times,
   const unsigned int begin_times
) {
   const double allTimes = single_times + begin_times;

   m_InitialStateDistribution[0] = single_times / allTimes;
   m_InitialStateDistribution[1] = begin_times / allTimes;
}

ModelHeader& MarkovChainModel::header() {
   return m_ModelHeader;
}

ModelHeader MarkovChainModel::header() const {
   return m_ModelHeader;
}

std::vector<uint8_t> MarkovChainModel::get_binary_model_data() const {
   BinaryModelHelper helper{*this};
   helper.run();
   return std::move(helper.get_data_ref());
}
