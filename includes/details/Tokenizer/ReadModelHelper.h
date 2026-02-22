//
// Created by QinBu_Rua on 2026/2/7.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZER_READMODELHELPER_H
#define AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZER_READMODELHELPER_H

#include <string>
#include <vector>

#include "classes/MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine::details::tokenizer {

class ReadModelHelper {
public:
   ReadModelHelper() = default;
   explicit ReadModelHelper(const std::string& filename);

   void clear();
   void set_model_file(const std::string& filename);
   void run();
   MarkovChainModel& get_model_ref();

   ReadModelHelper(const ReadModelHelper&)            = delete;
   ReadModelHelper(ReadModelHelper&&)                 = delete;
   ReadModelHelper& operator=(const ReadModelHelper&) = delete;
   ReadModelHelper& operator=(ReadModelHelper&&)      = delete;

private:
   static const uint32_t MODEL_MIN_SIZE;

   std::string m_FileName;
   std::vector<std::byte> m_RawData;
   std::vector<std::byte>::const_iterator m_Iterator;
   ModelHeader m_Header;
   MarkovChainModel m_MarkovModel;

private:
   void f_initialize();
   void f_parse_header();
   void f_parse_model();
   void f_complete();

};

}


#endif //AUTO_SORTING_MACHINE_INCLUDES_DETAILS_TOKENIZER_READMODELHELPER_H
