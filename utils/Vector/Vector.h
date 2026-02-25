//
// Created by QinBu_Rua on 2026/2/25.
//

#ifndef AUTO_SORTING_MACHINE_UTILS_VECTOR_VECTOR_H
#define AUTO_SORTING_MACHINE_UTILS_VECTOR_VECTOR_H

#include <array>
#include <cstdint>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace QinBuRua::auto_sorting_machine::utils {

template<typename T, uint32_t N = 0>
class Vector {
public:
   using Container = std::conditional_t<
      N == 0, std::vector<T>, std::array<T, N>
   >;

   struct DynamicTag {
   };

   struct StaticTag {
   };

public:
   T& at(uint32_t index);
   T& operator[](uint32_t index);
   T& front();
   T& back();
   T* data();

   T at(uint32_t index) const;
   T operator[](uint32_t index) const;
   T front() const;
   T back() const;
   const T* data() const;

   Container::iterator begin();
   Container::iterator end();
   Container::reverse_iterator rbegin();
   Container::reverse_iterator rend();

   Container::const_iterator cbegin() const;
   Container::const_iterator cend() const;
   Container::const_reverse_iterator crbegin() const;
   Container::const_reverse_iterator crend() const;

   [[nodiscard]] bool empty() const;
   [[nodiscard]] uint32_t size() const;
   [[nodiscard]] uint32_t max_size() const;

   void clear();
   void swap(Vector& vec) noexcept;
   void fill(const T& value);

   Vector();
   explicit Vector(const T& value);

   explicit Vector(uint32_t size);
   Vector(uint32_t size, const T& value);

   Vector(Vector&&)                 = default;
   Vector(const Vector&)            = default;
   Vector& operator=(Vector&&)      = default;
   Vector& operator=(const Vector&) = default;

   auto operator<=>(const Vector& vec) const = default;

public:
   static Vector of_size(uint32_t size);
   static Vector from_value(const T& value);

private:
   Vector(DynamicTag, uint32_t size);
   Vector(StaticTag, const T& value);

private:
   Container m_Data;
};

#include "Vector.inl"

}

#endif //AUTO_SORTING_MACHINE_UTILS_VECTOR_VECTOR_H
