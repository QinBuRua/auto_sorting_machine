//
// Created by QinBu_Rua on 2026/1/3.
//

#include <algorithm>
#include <ranges>
#include <string>
#include <utility>

#include "CharTypeArray.h"

namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine;

const std::string QinBuRua::auto_sorting_machine::CHARTYPE_TO_STR[4] = {
   "SINGLE",
   "BEGIN",
   "MIDDLE",
   "END"
};

CharTypeArray::CharTypeArray() {
   m_ByteCapacity = 0;
   m_Size         = 0;
   m_Data         = nullptr;
}

CharTypeArray::CharTypeArray(uint32_t size) {
   m_Size         = size;
   m_ByteCapacity = (size + 3) / 4;
   m_Data         = std::move(
      std::unique_ptr<uint8_t[]>(new uint8_t[m_ByteCapacity]())
   );
}

CharTypeArray::CharTypeArray(const CharTypeArray& other) {
   if (other.empty()) {
      m_ByteCapacity = 0;
      m_Size         = 0;
      m_Data         = nullptr;
      return;
   }
   m_Size         = other.m_Size;
   m_ByteCapacity = (m_Size + 3) / 4;
   m_Data         = std::move(
      std::unique_ptr<uint8_t[]>(new uint8_t[m_ByteCapacity])
   );
   ranges::copy_n(other.m_Data.get(), m_ByteCapacity, m_Data.get());
}

CharTypeArray::CharTypeArray(CharTypeArray&& other) noexcept {
   m_Size               = other.m_Size;
   m_ByteCapacity       = other.m_ByteCapacity;
   m_Data               = std::move(other.m_Data);
   other.m_Size         = 0;
   other.m_ByteCapacity = 0;
   other.m_Data         = nullptr;
}

CharTypeArray& CharTypeArray::operator=(const CharTypeArray& other) {
   if (&other == this) {
      return *this;
   }
   const auto otherUsedBytes = (other.m_Size + 3) / 4;
   if (otherUsedBytes == 0) {
      clear();
      return *this;
   }
   if (otherUsedBytes > m_ByteCapacity) {
      m_ByteCapacity = otherUsedBytes;
      m_Size         = other.m_Size;
      m_Data         = std::move(
         std::unique_ptr<uint8_t[]>(new uint8_t[m_ByteCapacity])
      );
      ranges::copy_n(other.m_Data.get(), m_ByteCapacity, m_Data.get());
      return *this;
   }
   const auto usedBytes = (m_Size + 3) / 4;
   ranges::copy_n(
      other.m_Data.get(),
      otherUsedBytes,
      m_Data.get()
   );
   for (uint32_t i = otherUsedBytes; i < usedBytes; ++i) {
      m_Data[i] = 0;
   }
   m_Size = other.m_Size;
   return *this;
}

CharTypeArray& CharTypeArray::operator=(CharTypeArray&& other) noexcept {
   m_Size               = other.m_Size;
   m_ByteCapacity       = other.m_ByteCapacity;
   m_Data               = std::move(other.m_Data);
   other.m_Size         = 0;
   other.m_ByteCapacity = 0;
   other.m_Data         = nullptr;
   return *this;
}

bool CharTypeArray::empty() const {
   return m_Size == 0;
}

uint32_t CharTypeArray::size() const {
   return m_Size;
}

uint32_t CharTypeArray::capacity() const {
   return m_ByteCapacity;
}

CharType CharTypeArray::get(const uint32_t index) const {
   return static_cast<CharType>(
      m_Data[index / 4] >> (index % 4 * 2) & 0b11
   );
}

CharType CharTypeArray::get_front() const {
   return static_cast<CharType>(
      m_Data[0] & 0b11
   );
}

CharType CharTypeArray::get_back() const {
   auto backIndex = m_Size - 1;
   return static_cast<CharType>(
      m_Data[backIndex / 4] >> (backIndex % 4 * 2) & 0b11
   );
}

void CharTypeArray::set(uint32_t index, CharType value) {
   m_Data[index / 4] &= ~(0b11 << (index % 4 * 2));
   m_Data[index / 4] |= std::to_underlying(value) << (index % 4 * 2);
}

void CharTypeArray::set_front(CharType value) {
   set(0, value);
}

void CharTypeArray::set_back(CharType value) {
   set(m_Size - 1, value);
}

void CharTypeArray::push_back(CharType value) {
   if (m_Size == 0) {
      m_Size         = 1;
      m_ByteCapacity = 1;
      m_Data         = std::make_unique<uint8_t[]>(m_ByteCapacity);
      m_Data[0]      = std::to_underlying(value);
      return;
   }
   if (m_Size >= m_ByteCapacity * 4) {
      auto tmp = std::make_unique<uint8_t[]>(m_ByteCapacity * 2);
      ranges::copy_n(m_Data.get(), m_ByteCapacity, tmp.get());
      m_Data         = std::move(tmp);
      m_ByteCapacity *= 2;
   }
   m_Data[m_Size / 4] |= std::to_underlying(value) << (m_Size % 4 * 2);
   ++m_Size;
}

void CharTypeArray::clear() {
   for (uint32_t i = 0; i < (m_Size + 3) / 4; ++i) {
      m_Data[i] = 0;
   }
   m_Size = 0;
}

void CharTypeArray::destroy() {
   m_Size         = 0;
   m_ByteCapacity = 0;
   m_Data         = nullptr;
}

void CharTypeArray::reserve(uint32_t elements_capacity) {
   uint32_t bytes = (elements_capacity + 3) / 4;
   if (bytes <= m_ByteCapacity) {
      return;
   }
   auto tmp = std::unique_ptr<uint8_t[]>(new uint8_t[bytes]());
   ranges::copy_n(m_Data.get(), (m_Size + 3) / 4, tmp.get());
   m_Data         = std::move(tmp);
   m_ByteCapacity = bytes;
}

void CharTypeArray::resize(uint32_t size) {
   if (size < m_Size) {
      auto end = (m_Size + 3) / 4;
      for (uint32_t i = (size + 3) / 4; i <= end; ++i) {
         m_Data[i] = 0;
      }
      for (uint32_t i = size; i < m_Size; ++i) {
         set(i, static_cast<CharType>(0));
      }
      m_Size = size;
      return;
   }
   auto reqBytes = (size + 3) / 4;
   if (reqBytes > m_ByteCapacity) {
      auto tmp = std::unique_ptr<uint8_t[]>(new uint8_t[reqBytes]());
      ranges::copy_n(m_Data.get(), (m_Size + 3) / 4, tmp.get());
      m_Data = std::move(tmp);
   }
   m_Size = size;
}

void CharTypeArray::shrink_to_fit() {
   if (m_Size == 0) {
      m_Data = nullptr;
      return;
   }
   auto bytes = (m_Size + 3) / 4;
   auto tmp   = std::unique_ptr<uint8_t[]>(new uint8_t[bytes]);
   ranges::copy_n(m_Data.get(), bytes, tmp.get());
   m_Data         = std::move(tmp);
   m_ByteCapacity = bytes;
}

std::string CharTypeArray::dump_sc() const {
   if (m_Size == 0) {
      return {};
   }
   std::string result;
   result.reserve(m_Size);
   for (uint32_t i = 0; i < m_Size; ++i) {
      result.push_back(m_CHARTYPE_TO_SINGLE_CHAR[std::to_underlying(get(i))]);
   }
   return result;
}

const char CharTypeArray::m_CHARTYPE_TO_SINGLE_CHAR[4] = {'S', 'B', 'M', 'E'};
