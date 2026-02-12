//
// Created by QinBu_Rua on 2026/2/7.
//

#ifndef AUTO_SORTING_MACHINE_READMODELHELPER_H
#define AUTO_SORTING_MACHINE_READMODELHELPER_H

#include <string>
#include <vector>

#include "classes/MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine::details::tokenizer {

class ReadModelHelper {
public:
   ReadModelHelper() = default;
   explicit ReadModelHelper(const std::string& filename);

   void clear();
   void set_model_file(const std::string& filename);//todo
   void run(); //todo

   ReadModelHelper(const ReadModelHelper&)            = delete;
   ReadModelHelper(ReadModelHelper&&)                 = delete;
   ReadModelHelper& operator=(const ReadModelHelper&) = delete;
   ReadModelHelper& operator=(ReadModelHelper&&)      = delete;

private:
   std::string m_FileName;
   uint32_t m_FileSize;
   std::vector<uint8_t> m_RawData;
   MarkovChainModel m_MarkovModel;

private:
   void f_initialize(); //todo
};

}


#endif //AUTO_SORTING_MACHINE_READMODELHELPER_H
