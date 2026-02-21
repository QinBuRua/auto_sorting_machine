//
// Created by QinBu_Rua on 2026/2/5.
//

#include <cstdint>
#include <ranges>

#include "BinaryHeaderHelper.h"

namespace stdr = std::ranges;
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

std::vector<std::byte>& BinaryHeaderHelper::get_data_ref() {
   return m_Data;
}

void BinaryHeaderHelper::f_initialize() {
   m_Data.resize(m_Header.get_need_capacity());
   m_Iter = m_Data.begin();
}

void BinaryHeaderHelper::f_binary_sha256() {
   auto bData = std::as_bytes(std::span{m_Header.m_Sha256});
   m_Iter    = stdr::copy(bData, m_Iter).out;
}

void BinaryHeaderHelper::f_binary_name() {
   const uint32_t len = m_Header.m_Name.length();

   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(&len),
      sizeof(len),
      m_Iter
   ).out;
   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(m_Header.m_Name.data()),
      len,
      m_Iter
   ).out;
}

void BinaryHeaderHelper::f_binary_version() {
   const uint32_t len = m_Header.m_Version.length();

   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(&len),
      sizeof(len),
      m_Iter
   ).out;
   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(m_Header.m_Version.data()),
      len,
      m_Iter
   ).out;
}

void BinaryHeaderHelper::f_binary_train_time() {
   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(&m_Header.m_TrainTime),
      sizeof(m_Header.m_TrainTime),
      m_Iter
   ).out;
}

void BinaryHeaderHelper::f_binary_dependency() {
   const auto& dependency = m_Header.m_Dependency;
   const uint32_t size    = dependency.size();
   m_Iter                 = stdr::copy_n(
      reinterpret_cast<const std::byte*>(&size),
      sizeof(size),
      m_Iter
   ).out;
   auto& iter = m_Iter;
   stdr::for_each(
      dependency, [&iter](const auto& dep) {
         const auto& [mtc, val] = dep;

         iter = stdr::copy_n(
            reinterpret_cast<const std::byte*>(&mtc),
            sizeof(mtc),
            iter
         ).out;
         iter = stdr::copy(val.get_binary_model_data(), iter).out;
      }
   );
}

void BinaryHeaderHelper::f_binary_description() {
   const uint32_t len = m_Header.m_Description.length();

   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(&len),
      sizeof(len),
      m_Iter
   ).out;
   m_Iter = stdr::copy_n(
      reinterpret_cast<const std::byte*>(m_Header.m_Description.data()),
      len,
      m_Iter
   ).out;
}
