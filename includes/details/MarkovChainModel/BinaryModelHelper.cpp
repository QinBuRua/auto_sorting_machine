//
// Created by QinBu_Rua on 2026/2/4.
//

#include <ranges>

#include "BinaryModelHelper.h"

#include "json.hpp"

namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::markov_chain_model;

BinaryModelHelper::BinaryModelHelper(const MarkovChainModel& model) : m_Model(model) {
}

void BinaryModelHelper::run() {
   f_initialize();
   f_binary_isd();
   f_binary_tp();
   f_binary_isd();
}

std::vector<uint8_t>& BinaryModelHelper::get_data_ref() {
   return m_Data;
}

void BinaryModelHelper::f_initialize() {
   m_IsdReqSize = sizeof(double) * 2;
   m_TpReqSize  = sizeof(double) * 4 * 4;
   m_EpReqSize  = m_Model.m_EmissionProbability.size() * (sizeof(wchar_t) + sizeof(double) * 4);
   m_SumReqSize = m_IsdReqSize + m_TpReqSize + m_EpReqSize + sizeof(decltype(m_Model.m_EmissionProbability.size()));

   m_Data.resize(m_SumReqSize);

   m_Iter = m_Data.begin();
}

void BinaryModelHelper::f_binary_isd() {
   m_Iter = std::copy_n(
      reinterpret_cast<const uint8_t*>(m_Model.m_InitialStateDistribution.data()),
      m_IsdReqSize,
      m_Iter
   );
}

void BinaryModelHelper::f_binary_tp() {
   auto& iter = m_Iter;
   std::ranges::for_each(
      m_Model.m_TransitionProbability,
      [&iter](const auto& arr)-> void {
         iter = std::copy_n(
            reinterpret_cast<const uint8_t*>(arr.data()),
            sizeof(double) * 4,
            iter
         );
      }
   );
}

void BinaryModelHelper::f_binary_ep() {
   auto pairs = m_Model.m_EmissionProbability
      | ranges::to<std::vector<std::pair<wchar_t, std::array<double, 4>>>>();
   ranges::sort(pairs);

   auto& iter = m_Iter;
   ranges::for_each(
      pairs,
      [&iter](const auto& pair)-> void {
         iter = std::copy_n(
            reinterpret_cast<const uint8_t*>(&pair.first),
            sizeof(wchar_t),
            iter
         );
         iter = std::copy_n(
            reinterpret_cast<const uint8_t*>(pair.second.data()),
            sizeof(double) * 4,
            iter
         );
      }
   );
}
