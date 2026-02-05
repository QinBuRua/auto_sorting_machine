//
// Created by QinBu_Rua on 2026/1/31.
//

#ifndef AUTO_SORTING_MACHINE_MODELHEADER_H
#define AUTO_SORTING_MACHINE_MODELHEADER_H

#include <array>
#include <cstdint>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "picosha2.h"

namespace QinBuRua::auto_sorting_machine {

enum class ModelMatching:uint8_t {
   SHA256      = 0b1,
   NAME        = 0b10,
   VERSION     = 0b100,
   TRAIN_TIME  = 0b1000,
   DEPENDENCE  = 0b10000,
   DESCRIPTION = 0b100000,
};

class ModelHeader {
   //todo
public:
   [[nodiscard]] size_t get_need_capacity() const;

   [[nodiscard]] std::array<uint8_t, 32> get_sha256() const;
   [[nodiscard]] std::string get_name() const;
   [[nodiscard]] std::string get_version() const;
   [[nodiscard]] std::string get_description() const;

   void set_sha256(const std::array<uint8_t, 32>& arr);
   void set_name(const std::string& name);
   void set_version(const std::string& version);
   void set_description(const std::string& description);
   void add_dependency(uint8_t matching_method, const ModelHeader& header);

   template<std::input_iterator Iter>
   void set_sha256_from(Iter begin, Iter end);
   template<std::ranges::input_range Ranges>
   void set_sha256_from(Ranges&& ranges);

private:
   std::array<uint8_t, 32> m_Sha256{};
   std::string m_Name;
   std::string m_Version;
   time_t m_TrainTime = 0;
   std::vector<std::pair<uint8_t/*匹配方式*/, ModelHeader>> m_Dependency{};
   std::string m_Description;

private:
   [[nodiscard]] size_t f_calculate_dependency_require_capacity() const;
};



#include "ModelHeader.inl"

} // QinBuRua::auto_sorting_machine

#endif //AUTO_SORTING_MACHINE_MODELHEADER_H
