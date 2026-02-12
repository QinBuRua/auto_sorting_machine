//
// Created by QinBu_Rua on 2026/2/7.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TOKENIZERTRAINER_H
#define AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TOKENIZERTRAINER_H

#include <array>
#include <stdfloat>
#include <string>
#include <vector>

#include "MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine {

class Tokenizer {
public:
   explicit Tokenizer(const MarkovChainModel& model);
   explicit Tokenizer(MarkovChainModel&& model) noexcept;

   void set_min_probability(std::float64_t probability);
   std::float64_t get_min_probability();

   void load(MarkovChainModel&& model) noexcept;
   void initialize();
   std::vector<std::u16string> tokenize(const std::u16string& text);

private:
   MarkovChainModel m_MarkovModel;
   std::array<std::float64_t, 4> m_DefaultPro;
   std::float64_t m_MinPro;

private:
   std::float64_t f_get_char_prob(char16_t ch, CharType type);
};

}


#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TOKENIZER_H
