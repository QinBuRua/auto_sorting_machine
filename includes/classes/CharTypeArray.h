//
// Created by QinBu_Rua on 2026/1/3.
//

#ifndef AUTO_SORTING_MACHINE_CHARTYPEARRAY_H
#define AUTO_SORTING_MACHINE_CHARTYPEARRAY_H

#include <charconv>
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
   CharTypeArray(size_t size);
   CharTypeArray(const CharTypeArray& other);
   CharTypeArray(CharTypeArray&& other) noexcept;

   CharTypeArray& operator=(const CharTypeArray& other);
   CharTypeArray& operator=(CharTypeArray&& other) noexcept;

   [[nodiscard]] bool empty() const;
   [[nodiscard]] size_t size() const;
   [[nodiscard]] size_t capacity() const;

   [[nodiscard]] CharType get(size_t index) const;
   CharType get_front() const;
   CharType get_back() const;
   void set(size_t index, CharType value);
   void set_front(CharType value);
   void set_back(CharType value);
   void push_back(CharType value);

   void clear();
   void destroy();
   void reserve(size_t elementsCapacity);
   void resize(size_t size);
   void shrink_to_fit();
   std::string dump_sc() const;

private:
   size_t m_Size;
   size_t m_ByteCapacity;
   std::unique_ptr<uint8_t[]> m_Data;

private:
   static const char m_CHARTYPE_TO_SINGLE_CHAR[4];
};

}

#endif //AUTO_SORTING_MACHINE_CHARTYPEARRAY_H
