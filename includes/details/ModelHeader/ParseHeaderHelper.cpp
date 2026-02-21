//
// Created by QinBu_Rua on 2026/2/8.
//

#include <ranges>

#include "ParseHeaderHelper.h"

namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::model_header;

ParseHeaderHelper::ParseHeaderHelper() {
}

ParseHeaderHelper::ParseHeaderHelper(const std::vector<std::byte>::const_iterator& begin) {
   load(begin);
}

void ParseHeaderHelper::load(const std::vector<std::byte>::const_iterator& begin) {
   m_Header.clear();
   m_Iter = begin;
}

void ParseHeaderHelper::run() {
   f_parse_sha256();
   f_parse_name();
   f_parse_version();
   f_parse_train_time();
   f_parse_dependency();
   f_parse_description();
}

QinBuRua::auto_sorting_machine::ModelHeader& ParseHeaderHelper::get_header_ref() {
   return m_Header;
}

std::vector<std::byte>::const_iterator ParseHeaderHelper::get_iter() const {
   return m_Iter;
}

void ParseHeaderHelper::f_parse_sha256() {
   m_Iter = ranges::copy_n(
      m_Iter,
      m_Header.m_Sha256.size(),
      reinterpret_cast<std::byte*>(m_Header.m_Sha256.data())
   ).in;
}

void ParseHeaderHelper::f_parse_name() {
   uint32_t size;
   m_Iter = ranges::copy_n(m_Iter, sizeof(uint32_t), reinterpret_cast<std::byte*>(&size)).in;
   if (size == 0) {
      return;
   }
   m_Header.m_Name.resize(size);
   m_Iter = ranges::copy_n(
      m_Iter,
      size,
      reinterpret_cast<std::byte*>(m_Header.m_Name.data())
   ).in;
}

void ParseHeaderHelper::f_parse_version() {
   uint32_t size;
   m_Iter = ranges::copy_n(
      m_Iter,
      sizeof(uint32_t),
      reinterpret_cast<std::byte*>(&size)
   ).in;
   if (size == 0) {
      return;
   }
   m_Header.m_Version.resize(size);
   m_Iter = ranges::copy_n(
      m_Iter,
      size,
      reinterpret_cast<std::byte*>(m_Header.m_Version.data())
   ).in;
}

void ParseHeaderHelper::f_parse_train_time() {
   m_Iter = ranges::copy_n(
      m_Iter,
      sizeof(m_Header.m_TrainTime),
      reinterpret_cast<std::byte*>(&m_Header.m_TrainTime)
   ).in;
}

void ParseHeaderHelper::f_parse_dependency() {
   ParseHeaderHelper helper;
   uint32_t num;
   m_Iter = ranges::copy_n(
      m_Iter,
      sizeof(uint32_t),
      reinterpret_cast<std::byte*>(&num)
   ).in;
   if (num == 0) {
      return;
   }
   auto& dependency = m_Header.m_Dependency;
   dependency.resize(num);
   for (uint32_t i = 0; i < num; ++i) {
      dependency[i].first = std::bit_cast<uint8_t>(*m_Iter);
      ++m_Iter;
      helper.load(m_Iter);
      helper.run();
      dependency[i].second = std::move(helper.get_header_ref());
   }
}

void ParseHeaderHelper::f_parse_description() {
   uint32_t size;
   m_Iter = ranges::copy_n(m_Iter, sizeof(uint32_t), reinterpret_cast<std::byte*>(&size)).in;
   if (size == 0) {
      return;
   }
   m_Header.m_Description.resize(size);
   m_Iter = ranges::copy_n(
      m_Iter,
      size,
      reinterpret_cast<std::byte*>(m_Header.m_Description.data())
   ).in;
}
