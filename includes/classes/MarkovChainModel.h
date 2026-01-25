//
// Created by QinBu_Rua on 2026/1/17.
//

#ifndef AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H
#define AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H

#include <array>
#include <unordered_map>

#include "CharTypeArray.h"

namespace QinBuRua::auto_sorting_machine {

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

private:
   double m_InitialStateDistribution[4] = {0.0f, 0.0f, 0.0f, 0.0f};
   double m_TransitionProbability[4][4] = {
      {0.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 0.0f}
   };
   std::unordered_map<wchar_t, std::array<double, 4>> m_EmissionProbability;

};

}
#endif //AUTO_SORTING_MACHINE_MARKOVCHAINMODEL_H
