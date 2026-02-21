//
// Created by QinBu_Rua on 2026/2/4.
//

#include <ranges>
#include <stdfloat>

#include "BinaryModelHelper.h"

#include "json.hpp"

using std::float64_t;
namespace stdr = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::markov_chain_model;

BinaryModelHelper::BinaryModelHelper(const MarkovChainModel& model) : m_Model(model) {
}

void BinaryModelHelper::run() {
   f_initialize();
   f_binary_isd();
   f_binary_tp();
   f_binary_ep();
}

std::vector<std::byte>& BinaryModelHelper::get_data_ref() {
   return m_Data;
}

void BinaryModelHelper::f_initialize() {
   m_IsdReqSize = sizeof(float64_t) * 2;
   m_TpReqSize  = sizeof(float64_t) * 4 * 4;
   m_EpReqSize  = sizeof(uint32_t) + m_Model.m_EmissionProbability.size() * (sizeof(wchar_t) + sizeof(float64_t) * 4);
   m_SumReqSize = m_IsdReqSize + m_TpReqSize + m_EpReqSize + sizeof(decltype(m_Model.m_EmissionProbability.size()));

   m_Data.resize(m_SumReqSize);

   m_Iter = m_Data.begin();
}

void BinaryModelHelper::f_binary_isd() {
   m_Iter = std::copy_n(
      reinterpret_cast<const std::byte*>(m_Model.m_InitialStateDistribution.data()),
      m_IsdReqSize,
      m_Iter
   );
}

void BinaryModelHelper::f_binary_tp() {
   auto& iter = m_Iter;
   stdr::for_each(
      m_Model.m_TransitionProbability,
      [&iter](const auto& arr)-> void {
         iter = std::copy_n(
            reinterpret_cast<const std::byte*>(arr.data()),
            sizeof(float64_t) * 4,
            iter
         );
      }
   );
}

void BinaryModelHelper::f_binary_ep() {
   auto pairs = m_Model.m_EmissionProbability
      | stdr::to<std::vector<std::pair<wchar_t, std::array<float64_t, 4>>>>();
   stdr::sort(pairs);

   const uint32_t size = pairs.size();
   m_Iter              = stdr::copy_n(
      reinterpret_cast<const std::byte*>(&size),
      sizeof(size),
      m_Iter
   ).out;
   auto& iter = m_Iter;
   stdr::for_each(
      pairs,
      [&iter](const auto& pair)-> void {
         iter = std::copy_n(
            reinterpret_cast<const std::byte*>(&pair.first),
            sizeof(wchar_t),
            iter
         );
         iter = std::copy_n(
            reinterpret_cast<const std::byte*>(pair.second.data()),
            sizeof(float64_t) * 4,
            iter
         );
      }
   );
}
