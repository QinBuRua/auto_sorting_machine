//
// Created by QinBu_Rua on 2026/2/5.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_DETAILS_MODELHEADER_BINARYHEADERHELPER_H
#define AUTO_SORTING_MACHINE_INCLUDES_DETAILS_MODELHEADER_BINARYHEADERHELPER_H

#include <vector>

#include "classes/ModelHeader.h"

namespace QinBuRua::auto_sorting_machine::details::model_header {

class BinaryHeaderHelper {
public:
   explicit BinaryHeaderHelper(const ModelHeader& header);

   void run();
   std::vector<std::byte>& get_data_ref();

   BinaryHeaderHelper()                                     = delete;
   BinaryHeaderHelper(const BinaryHeaderHelper&)            = delete;
   BinaryHeaderHelper(BinaryHeaderHelper&&)                 = delete;
   BinaryHeaderHelper& operator=(const BinaryHeaderHelper&) = delete;
   BinaryHeaderHelper& operator=(BinaryHeaderHelper&&)      = delete;

private:
   const ModelHeader& m_Header;
   std::vector<std::byte> m_Data;
   std::vector<std::byte>::iterator m_Iter;

private:
   void f_initialize();
   void f_binary_sha256();
   void f_binary_name();
   void f_binary_version();
   void f_binary_train_time();
   void f_binary_dependency();
   void f_binary_description();
};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_DETAILS_MODELHEADER_BINARYHEADERHELPER_H
