//
// Created by QinBu_Rua on 2026/2/7.
//

#include <ranges>

#include "Tokenizer.h"
#include "details/Tokenizer/ReadModelHelper.h"
#include "Logger/Logger.h"

namespace ranges = std::ranges;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;

Tokenizer::Tokenizer(const MarkovChainModel& model) {
   m_MarkovModel = model;
}

Tokenizer::Tokenizer(MarkovChainModel&& model) noexcept {
   m_MarkovModel = std::move(model);
}

void Tokenizer::load(MarkovChainModel&& model) noexcept {
   m_MarkovModel = std::move(model);
}

void Tokenizer::initialize() {
   m_DefaultPro = m_MarkovModel.calculate_default_ep_probabilities();
}

void Tokenizer::set_min_probability(std::float64_t probability) {
   if (probability >= 1) {
      slog::error_throw<std::logic_error>(slog::Tag{}, "The min probability must less than 1");
   }
   m_MinPro = probability;
}

std::float64_t Tokenizer::get_min_probability() {
   return m_MinPro;
}

std::vector<std::u16string> Tokenizer::tokenize(const std::u16string& text) {
   //todo:测试
   std::vector<std::u16string> words;
   CharType lastType = m_MarkovModel.get_isd(CharType::SINGLE) * f_get_char_prob(text.front(), CharType::SINGLE)
                       > m_MarkovModel.get_isd(CharType::BEGIN) * f_get_char_prob(text.front(), CharType::BEGIN)
                          ? CharType::SINGLE
                          : CharType::BEGIN;
   words.push_back(std::u16string{text.front()});

   ranges::for_each(
      text.begin() + 1, text.end(), [this,&words,&lastType](const auto& ch) {
         std::array<std::float64_t, 4> prob;
         for (uint8_t i = 0; i < 4; ++i) {
            auto thisType = static_cast<CharType>(i);
            prob[i]       = f_get_char_prob(ch, thisType)
               * m_MarkovModel.get_tp(lastType, thisType);
         }
         const auto maxIter = ranges::max_element(prob);
         lastType           = static_cast<CharType>(std::distance(prob.begin(), maxIter));

         if (lastType == CharType::BEGIN || lastType == CharType::SINGLE) {
            words.push_back(std::u16string{ch});
         } else {
            words.back().push_back(ch);
         }
      }
   );

   return words;
}

std::float64_t Tokenizer::f_get_char_prob(char16_t ch, CharType type) {
   if (!m_MarkovModel.has_ep(ch)) {
      return m_DefaultPro[std::to_underlying(type)];
   }
   return m_MarkovModel.get_ep(ch, type) == 0
             ? m_MinPro
             : m_MarkovModel.get_ep(ch, type);
}
