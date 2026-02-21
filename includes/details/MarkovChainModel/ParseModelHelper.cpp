//
// Created by QinBu_Rua on 2026/2/10.
//

#include <cstdint>
#include <ranges>
#include <stdfloat>

#include "ParseModelHelper.h"

using std::float64_t;
namespace stdr = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::markov_chain_model;

ParseModelHelper::ParseModelHelper(const std::vector<std::byte>::const_iterator& begin) {
   load(begin);
}

void ParseModelHelper::load(const std::vector<std::byte>::const_iterator& begin) {
   m_Model.clear();
   m_Iter = begin;
}

void ParseModelHelper::run() {
   f_parse_isd();
   f_parse_tp();
   f_parse_ep();
}

QinBuRua::auto_sorting_machine::MarkovChainModel& ParseModelHelper::get_model_ref() {
   return m_Model;
}

void ParseModelHelper::f_parse_isd() {
   m_Iter = stdr::copy_n(
      m_Iter,
      sizeof(float64_t) * 2,
      reinterpret_cast<std::byte*>(m_Model.m_InitialStateDistribution.data())
   ).in;
}

void ParseModelHelper::f_parse_tp() {
   auto& iter = m_Iter;
   stdr::for_each(
      m_Model.m_TransitionProbability, [&iter](auto& arr) {
         iter = stdr::copy_n(
            iter,
            sizeof(float64_t) * 4,
            reinterpret_cast<std::byte*>(arr.data())
         ).in;
      }
   );
}

void ParseModelHelper::f_parse_ep() {
   uint32_t size;
   m_Iter = stdr::copy_n(m_Iter, sizeof(size), reinterpret_cast<std::byte*>(&size)).in;
   m_Model.m_EmissionProbability.rehash(size);
   for (int i = 0; i < size; ++i) {
      wchar_t wch;
      m_Iter = stdr::copy_n(m_Iter, sizeof(wch), reinterpret_cast<std::byte*>(&wch)).in;
      m_Iter = stdr::copy_n(
         m_Iter,
         sizeof(float64_t) * 4,
         reinterpret_cast<std::byte*>(m_Model.m_EmissionProbability[wch].data())
      ).in;
   }
}
