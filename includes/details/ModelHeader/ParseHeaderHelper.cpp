//
// Created by QinBu_Rua on 2026/2/8.
//

#include <ranges>

#include "ParseHeaderHelper.h"

namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine::details::model_header;

ParseHeaderHelper::ParseHeaderHelper(std::span<const uint8_t> data) {
   load(data);
}

void ParseHeaderHelper::load(std::span<const uint8_t> data) {
   m_Header.clear();
   m_RawData = data;
   m_Iter    = m_RawData.begin();
}

void ParseHeaderHelper::run() {

}

void ParseHeaderHelper::f_parse_sha256() {
   m_Iter = ranges::copy_n(m_Iter, 32, m_Header.m_Sha256.begin()).in;
}

void ParseHeaderHelper::f_parse_name() {
   size_t size;

}
