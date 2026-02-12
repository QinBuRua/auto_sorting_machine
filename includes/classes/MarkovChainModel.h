//
// Created by QinBu_Rua on 2026/1/17.
//

#ifndef AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H
#define AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H

#include <array>
#include <stdfloat>
#include <unordered_map>
#include <vector>

#include "CharTypeArray.h"
#include "ModelHeader.h"

namespace QinBuRua::auto_sorting_machine {
namespace details::markov_chain_model {
class BinaryModelHelper;
class ParseModelHelper;
}

class MarkovChainModel {
public:
   MarkovChainModel();

   bool has_ep(char16_t charType) const;
   void clear();

   std::float64_t get_isd(CharType charType) const;
   std::float64_t get_tp(CharType from, CharType to) const;
   std::float64_t get_ep(char16_t wch, CharType tp) const;

   void set_isd(CharType charType, std::float64_t probability);
   void set_tp(CharType from, CharType to, std::float64_t probability);
   void set_ep(char16_t wch, CharType tp, std::float64_t probability);
   void set_isd_s(unsigned int single_times, unsigned int begin_times);

   std::array<std::float64_t, 4> calculate_default_ep_probabilities() const;

   ModelHeader& header();
   ModelHeader header() const;
   std::vector<uint8_t> get_binary_data() const;

private:
   ModelHeader m_ModelHeader{};
   std::array<std::float64_t, 2> m_InitialStateDistribution{};
   std::array<std::array<std::float64_t, 4>, 4> m_TransitionProbability{};
   std::unordered_map<char16_t, std::array<std::float64_t, 4>> m_EmissionProbability;

   friend class details::markov_chain_model::BinaryModelHelper;
   friend class details::markov_chain_model::ParseModelHelper;

private:
   void f_initialize_header();

};

}
#endif //AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H
