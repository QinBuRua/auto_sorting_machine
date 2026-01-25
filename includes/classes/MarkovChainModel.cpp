//
// Created by QinBu_Rua on 2026/1/17.
//

#include <algorithm>
#include <utility>

#include "MarkovChainModel.h"

using namespace QinBuRua::auto_sorting_machine;

bool MarkovChainModel::has_EP(wchar_t charType) const {
   return m_EmissionProbability.contains(charType);
}

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

std::vector<uint8_t> MarkovChainModel::get_binary_model_data() const {
   std::vector<uint8_t> result;

   constexpr size_t isdReqSize = sizeof(double) * 2;
   constexpr size_t tpReqSize  = sizeof(double) * 4 * 4;
   const size_t epReqSize      = m_EmissionProbability.size() * (sizeof(wchar_t) + sizeof(double) * 4);
   const size_t sumReqSize     = isdReqSize + tpReqSize + epReqSize + sizeof(decltype(m_EmissionProbability.size()));
   result.resize(sumReqSize);

   std::copy_n(
      reinterpret_cast<const uint8_t*>(m_InitialStateDistribution),
      isdReqSize,
      result.data()
   );
   std::copy_n(
      reinterpret_cast<const uint8_t*>(m_TransitionProbability),
      tpReqSize,
      result.data() + isdReqSize
   );
   size_t index          = isdReqSize + tpReqSize;
   const auto size_of_ep = m_EmissionProbability.size();
   std::copy_n(
      reinterpret_cast<const uint8_t*>(&size_of_ep),
      sizeof(decltype(size_of_ep)),
      result.data() + index
   );
   index += sizeof(decltype(size_of_ep));

   for (const auto& [key, value]: m_EmissionProbability) {
      std::copy_n(
         reinterpret_cast<const uint8_t*>(&key),
         sizeof(decltype(key)),
         result.data() + index
      );
      index += sizeof(decltype(key));
      std::copy_n(
         reinterpret_cast<const uint8_t*>(value.data()),
         sizeof(double) * 4,
         result.data() + index
      );
      index += sizeof(double) * 4;
   }

   return result;
}
