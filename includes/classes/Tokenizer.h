//
// Created by QinBu_Rua on 2026/2/7.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TOKENIZER_H
#define AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TOKENIZER_H

#include <array>
#include <stdfloat>
#include <string>
#include <vector>

#include "MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine {

class Tokenizer {
public:
   using Word     = std::u16string;
   using Sentence = std::u16string;

public:
   explicit Tokenizer(const MarkovChainModel& model);
   explicit Tokenizer(MarkovChainModel&& model) noexcept;
   explicit Tokenizer(const std::string& filename);

   void set_min_probability(std::float64_t probability);
   std::float64_t get_min_probability() const;

   void clear();
   void load(MarkovChainModel&& model) noexcept;
   void load_from_file(const std::string& filename);
   void initialize();
   std::vector<Word> tokenize(const Sentence& text);

private:
   MarkovChainModel m_MarkovModel;
   std::array<std::float64_t, 4> m_DefaultPro;
   std::float64_t m_MinPro;

private:
   [[nodiscard]] static bool is_punctuation(char16_t ch);
   [[nodiscard]] static bool is_space(char16_t ch);
   [[nodiscard]] static bool is_all_space_or_punctuation(const Sentence& text);
   [[nodiscard]] static bool is_english(char16_t ch);
   [[nodiscard]] static bool is_number(char16_t ch);
   [[nodiscard]] static bool is_all_english_or_number(const Sentence& text);
   [[nodiscard]] static std::vector<Word> single_english_or_number_tokenize(const Sentence& text);

   [[nodiscard]] std::float64_t f_get_char_prob(char16_t ch, CharType type) const;
   [[nodiscard]] static std::vector<Sentence> f_preprocess(const Sentence& text);
   [[nodiscard]] std::vector<Word> f_single_chinese_tokenize(const Sentence& text);

};

}


#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TOKENIZER_H
