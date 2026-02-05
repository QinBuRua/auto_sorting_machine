//
// Created by QinBu_Rua on 2026/1/17.
//

#ifndef AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H
#define AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H

#include <array>
#include <unordered_map>
#include <vector>

#include "CharTypeArray.h"
#include "ModelHeader.h"

namespace QinBuRua::auto_sorting_machine {
namespace details::markov_chain_model {
class BinaryModelHelper;
}

class MarkovChainModel {
public:
   MarkovChainModel() = default;

   bool has_EP(wchar_t charType) const;

   double get_ISD(CharType charType) const;
   double get_TP(CharType from, CharType to) const;
   double get_EP(wchar_t wch, CharType tp) const;

   void set_ISD(CharType charType, double probability);
   void set_TP(CharType from, CharType to, double probability);
   void set_EP(wchar_t wch, CharType tp, double probability);

   void set_ISDs(unsigned int single_times, unsigned int begin_times);

   std::vector<uint8_t> get_binary_model_data() const;

private:
   ModelHeader m_ModelHeader; //todo
   std::array<double, 2> m_InitialStateDistribution{};
   std::array<std::array<double, 4>, 4> m_TransitionProbability{};
   std::unordered_map<wchar_t, std::array<double, 4>> m_EmissionProbability;

   friend class details::markov_chain_model::BinaryModelHelper;

};

}
#endif //AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H
