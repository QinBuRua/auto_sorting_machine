//
// Created by QinBu_Rua on 2026/2/4.
//

#ifndef AUTO_SORTING_MACHINE_BINARYMODELHELPER_H
#define AUTO_SORTING_MACHINE_BINARYMODELHELPER_H

#include <cstdint>
#include <vector>

#include "classes/MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine::details::markov_chain_model {

class BinaryModelHelper {
public:
   explicit BinaryModelHelper(const MarkovChainModel& model);

   void run();
   std::vector<uint8_t>& get_data_ref();

   BinaryModelHelper()                                    = delete;
   BinaryModelHelper(const BinaryModelHelper&)            = delete;
   BinaryModelHelper(BinaryModelHelper&&)                 = delete;
   BinaryModelHelper& operator=(const BinaryModelHelper&) = delete;
   BinaryModelHelper& operator=(BinaryModelHelper&&)      = delete;

private:
   const MarkovChainModel& m_Model;
   std::vector<uint8_t> m_Data;
   std::vector<uint8_t>::iterator m_Iter;

   uint32_t m_IsdReqSize;
   uint32_t m_TpReqSize;
   uint32_t m_EpReqSize;
   uint32_t m_SumReqSize;

   void f_initialize();
   void f_binary_isd();
   void f_binary_tp();
   void f_binary_ep();
};

}



#endif //AUTO_SORTING_MACHINE_BINARYMODELHELPER_H
