//
// Created by QinBu_Rua on 2026/2/8.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_DETAILS_MODELHEADER_PARSEHEADERHELPER_H
#define AUTO_SORTING_MACHINE_INCLUDES_DETAILS_MODELHEADER_PARSEHEADERHELPER_H

#include <vector>

#include "classes/ModelHeader.h"

namespace QinBuRua::auto_sorting_machine::details::model_header {

class ParseHeaderHelper {
public:
   ParseHeaderHelper() = default;
   ParseHeaderHelper(const std::vector<std::byte>::const_iterator& begin);

   void load(const std::vector<std::byte>::const_iterator& begin);
   void run();
   ModelHeader& get_header_ref();
   [[nodiscard]] std::vector<std::byte>::const_iterator get_iter() const;

   ParseHeaderHelper(ParseHeaderHelper&&)                 = delete;
   ParseHeaderHelper(const ParseHeaderHelper&)            = delete;
   ParseHeaderHelper& operator=(ParseHeaderHelper&&)      = delete;
   ParseHeaderHelper& operator=(const ParseHeaderHelper&) = delete;

private:
   std::vector<std::byte>::const_iterator m_Iter;
   ModelHeader m_Header{};

   void f_parse_sha256();
   void f_parse_name();
   void f_parse_version();
   void f_parse_train_time();
   void f_parse_dependency();
   void f_parse_description();
};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_DETAILS_MODELHEADER_PARSEHEADERHELPER_H
