//
// Created by QinBu_Rua on 2026/1/31.
//

#include <cstdint>

#include "picosha2.h"

#include "ModelHeader.h"
#include "details/ModelHeader/BinaryHeaderHelper.h"

using namespace QinBuRua::auto_sorting_machine;

using picosha2::hash256;

uint32_t ModelHeader::get_need_capacity() const {
   constexpr auto sha256Need = 32;
   const auto nameNeed = sizeof(uint32_t) + m_Name.size();
   const auto versionNeed = sizeof(uint32_t) + m_Version.size();
   constexpr auto trainTimeNeed = sizeof(m_TrainTime);
   const auto dependenceNeed = f_calculate_dependency_require_capacity();
   const auto descriptionNeed = sizeof(uint32_t) + m_Description.size();
   const auto allNeed = sha256Need + nameNeed + versionNeed + trainTimeNeed + dependenceNeed + descriptionNeed;
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

uint64_t ModelHeader::get_train_time() const {
   return m_TrainTime;
}

std::string ModelHeader::get_description() const {
   return m_Description;
}

std::optional<const ModelHeader::DependencyType> ModelHeader::find_dependency(const std::string& name) const {
   for (const auto& dependency: m_Dependency) {
      if (name == dependency.second.get_name()) {
         return dependency;
      }
   }
   return std::nullopt;
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

void ModelHeader::set_train_time(const uint64_t& train_time) {
   m_TrainTime = train_time;
}

void ModelHeader::set_description(const std::string& description) {
   m_Description = description;
}

void ModelHeader::add_dependency(uint8_t matching_method, const ModelHeader& header) {
   m_Dependency.emplace_back();
   m_Dependency.back().first  = matching_method;
   m_Dependency.back().second = header;
}

const std::vector<ModelHeader::DependencyType>& ModelHeader::dependency() {
   return m_Dependency;
}

std::vector<std::byte> ModelHeader::get_binary_model_data() const {
   details::model_header::BinaryHeaderHelper helper{*this};
   helper.run();
   return std::move(helper.get_data_ref());
}

void ModelHeader::clear() {
   m_Sha256.fill(0);
   m_Name.clear();
   m_Version.clear();
   m_TrainTime = 0;
   m_Dependency.clear();
   m_Description.clear();
}

uint32_t ModelHeader::f_calculate_dependency_require_capacity() const {
   auto dependenceNeed = 0;
   std::ranges::for_each(
      m_Dependency,
      [&dependenceNeed](const auto& header) {
         dependenceNeed += sizeof(uint8_t) + header.second.get_need_capacity();
      }
   );
   return sizeof(uint32_t) + dependenceNeed;
}
