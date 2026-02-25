//
// Created by QinBu_Rua on 2026/2/25.
//

#ifndef AUTO_SORTING_MACHINE_UTILS_VECTOR_VECTOR_INL
#define AUTO_SORTING_MACHINE_UTILS_VECTOR_VECTOR_INL

template<typename T, uint32_t N>
T& Vector<T, N>::at(uint32_t index) {
   if (index >= m_Data.size()) {
      throw std::out_of_range("Vector index out of range");
   }
   return m_Data[index];
}

template<typename T, uint32_t N>
T& Vector<T, N>::operator[](uint32_t index) {
   return m_Data[index];
}

template<typename T, uint32_t N>
T& Vector<T, N>::front() {
   return m_Data.front();
}

template<typename T, uint32_t N>
T& Vector<T, N>::back() {
   return m_Data.back();
}

template<typename T, uint32_t N>
T* Vector<T, N>::data() {
   return m_Data.data();
}

template<typename T, uint32_t N>
T Vector<T, N>::at(uint32_t index) const {
   if (index >= m_Data.size()) {
      throw std::out_of_range("Vector index out of range");
   }
   return m_Data[index];
}

template<typename T, uint32_t N>
T Vector<T, N>::operator[](uint32_t index) const {
   return m_Data[index];
}

template<typename T, uint32_t N>
T Vector<T, N>::front() const {
   return m_Data.front();
}

template<typename T, uint32_t N>
T Vector<T, N>::back() const {
   return m_Data.back();
}

template<typename T, uint32_t N>
const T* Vector<T, N>::data() const {
   return m_Data.data();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::iterator Vector<T, N>::begin() {
   return m_Data.begin();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::iterator Vector<T, N>::end() {
   return m_Data.end();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::reverse_iterator Vector<T, N>::rbegin() {
   return m_Data.rbegin();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::reverse_iterator Vector<T, N>::rend() {
   return m_Data.rend();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::const_iterator Vector<T, N>::cbegin() const {
   return m_Data.cbegin();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::const_iterator Vector<T, N>::cend() const {
   return m_Data.cend();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::const_reverse_iterator Vector<T, N>::crbegin() const {
   return m_Data.crbegin();
}

template<typename T, uint32_t N>
std::conditional_t<N == 0, std::vector<T>, std::array<T, N>>::const_reverse_iterator Vector<T, N>::crend() const {
   return m_Data.crend();
}

template<typename T, uint32_t N>
bool Vector<T, N>::empty() const {
   return m_Data.empty();
}

template<typename T, uint32_t N>
uint32_t Vector<T, N>::size() const {
   return m_Data.size();
}

template<typename T, uint32_t N>
uint32_t Vector<T, N>::max_size() const {
   return m_Data.max_size();
}

template<typename T, uint32_t N>
void Vector<T, N>::clear() {
   std::ranges::fill(m_Data, T{});
}

template<typename T, uint32_t N>
void Vector<T, N>::swap(Vector& vec) noexcept {
   m_Data.swap(vec);
}

template<typename T, uint32_t N>
void Vector<T, N>::fill(const T& value) {
   std::ranges::fill(m_Data, value);
}

template<typename T, uint32_t N>
Vector<T, N>::Vector() {
   static_assert(N != 0, "Dynamic Vector must be sized!!!");
   m_Data.fill(T{});
}

template<typename T, uint32_t N>
Vector<T, N>::Vector(const T& value) {
   static_assert(N != 0, "Dynamic Vector must be sized!!!");
   m_Data.fill(value);
}

template<typename T, uint32_t N>
Vector<T, N>::Vector(uint32_t size) {
   static_assert(N == 0, "Static Vector can NOT be sized!!!");
   m_Data.resize(size);
}

template<typename T, uint32_t N>
Vector<T, N>::Vector(uint32_t size, const T& value) {
   static_assert(N == 0, "Static Vector can NOT be sized!!!");
   m_Data = Container{size, value};
}

template<typename T, uint32_t N>
Vector<T, N> Vector<T, N>::of_size(uint32_t size) {
   static_assert(N == 0, "Static Vector can NOT be sized!!!");
   return std::move(Vector{DynamicTag{}, size});
}

template<typename T, uint32_t N>
Vector<T, N> Vector<T, N>::from_value(const T& value) {
   static_assert(N != 0, "Dynamic Vector must be sized!!!");
   return std::move(Vector{StaticTag{}, value});
}

template<typename T, uint32_t N>
Vector<T, N>::Vector(DynamicTag, uint32_t size) : m_Data(size) {
}

template<typename T, uint32_t N>
Vector<T, N>::Vector(StaticTag, const T& value) {
   m_Data.fill(value);
}

#endif //AUTO_SORTING_MACHINE_UTILS_VECTOR_VECTOR_INL
