//
// Created by QinBu_Rua on 2026/2/10.
//

#ifndef AUTO_SORTING_MACHINE_PARSEMODELHELPER_H
#define AUTO_SORTING_MACHINE_PARSEMODELHELPER_H

#include <cstdint>
#include <vector>

#include "classes/MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine::details::markov_chain_model {

class ParseModelHelper {
public:
   ParseModelHelper() = default;
   explicit ParseModelHelper(const std::vector<uint8_t>::const_iterator& begin);

   void load(const std::vector<uint8_t>::const_iterator& begin);
   void run();
   MarkovChainModel& get_model_ref();

   ParseModelHelper(ParseModelHelper&&)                 = delete;
   ParseModelHelper(const ParseModelHelper&)            = delete;
   ParseModelHelper& operator=(ParseModelHelper&&)      = delete;
   ParseModelHelper& operator=(const ParseModelHelper&) = delete;

private:
   std::vector<uint8_t>::const_iterator m_Iter;
   MarkovChainModel m_Model;

private:
   void f_parse_isd();
   void f_parse_tp();
   void f_parse_ep();
};

}


#endif //AUTO_SORTING_MACHINE_PARSEMODELHELPER_H
