//
// Created by QinBu_Rua on 2026/1/31.
//

#include "picosha2.h"

#include "ModelHeader.h"

using namespace QinBuRua::auto_sorting_machine;

using picosha2::hash256;

size_t ModelHeader::get_need_capacity() const{
   auto size_tNeed      = sizeof(m_Dependency.size());
   auto sha256Need      = 32;
   auto nameNeed        = size_tNeed + m_Name.size();
   auto versionNeed     = size_tNeed + m_Version.size();
   auto trainTimeNeed   = sizeof(m_TrainTime);
   auto dependenceNeed  = f_calculate_dependency_require_capacity();
   auto descriptionNeed = size_tNeed + m_Description.size();
   auto allNeed         = sha256Need + nameNeed + versionNeed + trainTimeNeed + dependenceNeed + descriptionNeed;
   return allNeed;
}

std::array<uint8_t, 32> ModelHeader::get_sha256() const {
   return m_Sha256;
}

std::string ModelHeader::get_name() const {
   return m_Name;
}

std::string ModelHeader::get_version() const {
   return m_Version;
}

std::string ModelHeader::get_description() const {
   return m_Description;
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

void ModelHeader::add_dependency(uint8_t matching_method, const ModelHeader& header) {
   m_Dependency.emplace_back();
   m_Dependency.back().first = matching_method;
   m_Dependency.back().second = header;
}

size_t ModelHeader::f_calculate_dependency_require_capacity() const {
   auto size_tNeed     = sizeof(m_Dependency.size());
   auto dependenceNeed = 0;
   std::ranges::for_each(
      m_Dependency,
      [&dependenceNeed](const auto& header) {
         dependenceNeed += sizeof(uint8_t) + header.second.get_need_capacity();
      }
   );
   return size_tNeed + dependenceNeed;
}
