//
// Created by QinBu_Rua on 2026/1/31.
//

#include "picosha2.h"

#include "ModelHeader.h"

using namespace QinBuRua::auto_sorting_machine;

using picosha2::hash256;

size_t ModelHeader::get_need_capacity() const{
   auto size_tNeed      = sizeof(m_Dependence.size());
   auto sha256Need      = 32;
   auto nameNeed        = size_tNeed + m_Name.size();
   auto versionNeed     = size_tNeed + m_Version.size();
   auto trainTimeNeed   = sizeof(m_TrainTime);
   auto dependenceNeed  = f_calculate_dependence_need_capacity();
   auto descriptionNeed = size_tNeed + m_Description.size();
   auto allNeed         = sha256Need + nameNeed + versionNeed + trainTimeNeed + dependenceNeed + descriptionNeed;
   return allNeed;
}

void ModelHeader::set_sha256(const std::array<uint8_t, 32>& arr) {
   m_Sha256 = arr;
}

void ModelHeader::set_name(const std::string& name) {
   m_Name = name;
}

void ModelHeader::set_version(const std::string& version) {
   m_Version = version;
}

void ModelHeader::set_description(const std::string& description) {
   m_Description = description;
}

size_t ModelHeader::f_calculate_dependence_need_capacity() const {
   auto size_tNeed     = sizeof(m_Dependence.size());
   auto dependenceNeed = 0;
   std::ranges::for_each(
      m_Dependence,
      [&dependenceNeed](const auto& header) {
         dependenceNeed += sizeof(uint8_t) + header.second.get_need_capacity();
      }
   );
   return size_tNeed + dependenceNeed;
}
