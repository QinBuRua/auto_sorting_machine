//
// Created by QinBu_Rua on 2026/2/5.
//

#include <ranges>

#include "BinaryHeaderHelper.h"

namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::model_header;


BinaryHeaderHelper::BinaryHeaderHelper(const ModelHeader& header) : m_Header(header) {
}

void BinaryHeaderHelper::run() {
   f_initialize();
   f_binary_sha256();
   f_binary_name();
   f_binary_version();
   f_binary_train_time();
   f_binary_dependency();
   f_binary_description();
}

std::vector<uint8_t>& BinaryHeaderHelper::get_data_ref() {
   return m_Data;
}

void BinaryHeaderHelper::f_initialize() {
   m_Data.resize(m_Header.get_need_capacity());
   m_Iter = m_Data.begin();
}

void BinaryHeaderHelper::f_binary_sha256() {
   m_Iter = ranges::copy(m_Header.m_Sha256, m_Iter).out;
}

void BinaryHeaderHelper::f_binary_name() {
   const auto len = m_Header.m_Name.length();

   m_Iter = std::copy_n(
      reinterpret_cast<const uint8_t*>(&len),
      sizeof(len),
      m_Iter
   );
   m_Iter = ranges::copy(std::span{m_Header.m_Name}, m_Iter).out;
}

void BinaryHeaderHelper::f_binary_version() {
   const auto len = m_Header.m_Version.length();

   m_Iter = std::copy_n(
      reinterpret_cast<const uint8_t*>(&len),
      sizeof(len),
      m_Iter
   );
   m_Iter = ranges::copy(std::span{m_Header.m_Version}, m_Iter).out;
}

void BinaryHeaderHelper::f_binary_train_time() {
   m_Iter = std::copy_n(
      reinterpret_cast<const uint8_t*>(&m_Header.m_TrainTime),
      sizeof(m_Header.m_TrainTime),
      m_Iter
   );
}

void BinaryHeaderHelper::f_binary_dependency() {
   const auto& dependency = m_Header.m_Dependency;
   const auto size        = dependency.size();
   m_Iter                 = std::copy_n(
      reinterpret_cast<const uint8_t*>(&size),
      sizeof(size),
      m_Iter
   );
   auto& iter = m_Iter;
   ranges::for_each(
      dependency, [&iter](const auto& dep) {
         const auto& [mtc, val] = dep;

         iter = std::copy_n(
            reinterpret_cast<const uint8_t*>(&mtc),
            sizeof(mtc),
            iter
         );
         iter = ranges::copy(val.get_binary_model_data(), iter).out;
      }
   );
}

void BinaryHeaderHelper::f_binary_description() {
   const auto len = m_Header.m_Description.length();

   m_Iter = std::copy_n(
      reinterpret_cast<const uint8_t*>(&len),
      sizeof(len),
      m_Iter
   );
   m_Iter = ranges::copy(m_Header.m_Description, m_Iter).out;
}
