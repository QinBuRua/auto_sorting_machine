//
// Created by QinBu_Rua on 2026/1/3.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_CLASSES_CHARTYPEARRAY_H
#define AUTO_SORTING_MACHINE_INCLUDES_CLASSES_CHARTYPEARRAY_H

#include <cstdint>
#include <memory>
#include <string>

namespace QinBuRua::auto_sorting_machine {

enum class CharType :uint8_t {
   SINGLE = 0,
   BEGIN  = 1,
   MIDDLE = 2,
   END    = 3
};

extern const std::string CHARTYPE_TO_STR[4];

class CharTypeArray {
public:
   CharTypeArray();
   CharTypeArray(uint32_t size);
   CharTypeArray(const CharTypeArray& other);
   CharTypeArray(CharTypeArray&& other) noexcept;

   CharTypeArray& operator=(const CharTypeArray& other);
   CharTypeArray& operator=(CharTypeArray&& other) noexcept;

   [[nodiscard]] bool empty() const;
   [[nodiscard]] uint32_t size() const;
   [[nodiscard]] uint32_t capacity() const;

   [[nodiscard]] CharType get(uint32_t index) const;
   [[nodiscard]] CharType get_front() const;
   [[nodiscard]] CharType get_back() const;
   void set(uint32_t index, CharType value);
   void set_front(CharType value);
   void set_back(CharType value);
   void push_back(CharType value);

   void clear();
   void destroy();
   void reserve(uint32_t elements_capacity);
   void resize(uint32_t size);
   void shrink_to_fit();
   [[nodiscard]] std::string dump_sc() const;

private:
   uint32_t m_Size;
   uint32_t m_ByteCapacity;
   std::unique_ptr<uint8_t[]> m_Data;

private:
   static const char CHARTYPE_TO_SINGLE_CHAR[4];
};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_CHARTYPEARRAY_H
