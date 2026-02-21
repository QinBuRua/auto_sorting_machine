//
// Created by QinBu_Rua on 2026/1/17.
//

#include <algorithm>
#include <ranges>
#include <stdfloat>
#include <utility>

#include "MarkovChainModel.h"

#include <numeric>

#include "details/MarkovChainModel/BinaryModelHelper.h"

namespace ranges = std::ranges;
using std::float64_t;
using namespace QinBuRua::auto_sorting_machine;
using namespace details::markov_chain_model;

MarkovChainModel::MarkovChainModel() {
   f_initialize_header();
}

bool MarkovChainModel::has_ep(char16_t charType) const {
   return m_EmissionProbability.contains(charType);
}

void MarkovChainModel::clear() {
   m_ModelHeader.clear();
   m_InitialStateDistribution.fill(0);
   m_TransitionProbability.fill({});
   m_EmissionProbability.clear();
}

float64_t MarkovChainModel::get_isd(CharType charType) const {
   return m_InitialStateDistribution[std::to_underlying(charType)];
}

float64_t MarkovChainModel::get_tp(CharType from, CharType to) const {
   return m_TransitionProbability[std::to_underlying(from)][std::to_underlying(to)];
}

float64_t MarkovChainModel::get_ep(const char16_t wch, CharType tp) const {
   auto arr = m_EmissionProbability.find(wch);
   if (arr != m_EmissionProbability.end()) {
      return arr->second[std::to_underlying(tp)];
   } else {
      return 0.0;
   }
}

void MarkovChainModel::set_isd(CharType charType, const float64_t probability) {
   m_InitialStateDistribution[std::to_underlying(charType)] = probability;
}

void MarkovChainModel::set_tp(CharType from, CharType to, const float64_t probability) {
   m_TransitionProbability[std::to_underlying(from)][std::to_underlying(to)] = probability;
}

void MarkovChainModel::set_ep(const char16_t wch, CharType tp, const float64_t probability) {
   m_EmissionProbability[wch][std::to_underlying(tp)] = probability;
}

void MarkovChainModel::set_isd_s(
   const unsigned int single_times,
   const unsigned int begin_times
) {
   const float64_t allTimes = single_times + begin_times;

   m_InitialStateDistribution[0] = single_times / allTimes;
   m_InitialStateDistribution[1] = begin_times / allTimes;
}

std::array<std::float64_t, 4> MarkovChainModel::calculate_default_ep_probabilities() const {
   std::array<std::float64_t, 4> defaultProbabilities{};
   ranges::for_each(
      m_EmissionProbability, [&defaultProbabilities](const auto& pair) {
         for (uint32_t i = 0; i < 4; ++i) {
            defaultProbabilities[i] += pair.second[i];
         }
      }
   );
   std::float64_t sum = ranges::fold_left(
      defaultProbabilities,
      0,
      std::plus<std::float64_t>()
   );
   ranges::transform(
      defaultProbabilities,
      defaultProbabilities.begin(),
      [&sum](auto x) { return x / sum; }
   );
   return defaultProbabilities;
}

ModelHeader& MarkovChainModel::header() {
   return m_ModelHeader;
}

ModelHeader MarkovChainModel::header() const {
   return m_ModelHeader;
}

std::vector<std::byte> MarkovChainModel::get_binary_data() const {
   BinaryModelHelper helper{*this};
   helper.run();
   return std::move(helper.get_data_ref());
}

void MarkovChainModel::f_initialize_header() {
   m_ModelHeader.set_name("markov_chain_model");
   m_ModelHeader.set_train_time(time(nullptr));
}
