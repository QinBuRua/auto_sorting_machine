//
// Created by QinBu_Rua on 2026/2/7.
//

#include <ranges>
#include <unordered_set>

#include "Tokenizer.h"
#include "details/Tokenizer/ReadModelHelper.h"
#include "Logger/Logger.h"

namespace stdr = std::ranges;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;

Tokenizer::Tokenizer(const MarkovChainModel& model) : m_DefaultPro(), m_MinPro(0.05) {
   m_MarkovModel = model;
}

Tokenizer::Tokenizer(MarkovChainModel&& model) noexcept : m_DefaultPro(), m_MinPro(0.05) {
   m_MarkovModel = std::move(model);
}

void Tokenizer::set_min_probability(std::float64_t probability) {
   if (probability >= 1) {
      slog::error_throw_sl<std::logic_error>(slog::Tag{}, "The min probability must less than 1");
   }
   m_MinPro = probability;
}

std::float64_t Tokenizer::get_min_probability() const {
   return m_MinPro;
}

void Tokenizer::load(MarkovChainModel&& model) noexcept {
   m_MarkovModel = std::move(model);
}

void Tokenizer::initialize() {
   m_DefaultPro = m_MarkovModel.calculate_default_ep_probabilities();
}

std::vector<std::u16string> Tokenizer::tokenize(const std::u16string& text) {
   if (text.empty() || is_all_space_or_punctuation(text)) {
      return {};
   }
   std::vector<Sentence> preprocessedText = f_preprocess(text);
   std::vector<Word> words;
   stdr::for_each(
      preprocessedText, [this,&words](const Sentence& sentence)-> void {
         if (is_all_english_or_number(sentence)) {
            words.append_range(single_english_or_number_tokenize(sentence));
         } else {
            words.append_range(f_single_chinese_tokenize(sentence));
         }
      }
   );
   return words;
}

bool Tokenizer::is_punctuation(char16_t ch) {
   static const std::unordered_set PUNCTUATIONS = {
      // 英文/ASCII 标点 (32-47范围的部分常用符号)
      u'.', u',', u'!', u'?', u';', u':', u'-', u'\'', u'\"',
      u'(', u')', u'[', u']', u'{', u'}',
      // 中文标点 (位于BMP内的常用符号)
      u'。', u'，', u'！', u'？', u'；', u'：', u'“', u'”',
      u'‘', u'’', u'（', u'）', u'【', u'】', u'《', u'》',
      u'、', u'—', u'…', u'·'
   };
   return PUNCTUATIONS.contains(ch);
}

bool Tokenizer::is_space(char16_t ch) {
   static const std::unordered_set SPACE = {
      u' ',      // 空格 (U+0020)
      u'\t',     // 制表符 (U+0009)
      u'\n',     // 换行 (U+000A)
      u'\r',     // 回车 (U+000D)
      u'\f',     // 换页 (U+000C)
      u'\v',     // 垂直制表符 (U+000B)
      u'\u00A0', // 不换行空格 (U+00A0)
      u'\u3000', // 全角空格 (U+3000) - 中文空格
      u'\u2000', // 各种Unicode空格
      u'\u2001',
      u'\u2002',
      u'\u2003',
      u'\u2004',
      u'\u2005',
      u'\u2006',
      u'\u2007',
      u'\u2008',
      u'\u2009',
      u'\u200A',
      u'\u2028', // 行分隔符
      u'\u2029', // 段落分隔符
      u'\u202F', // 窄不换行空格
      u'\u205F', // 中等数学空格
      u'\u3000', // 表意文字空格
   };
   return SPACE.contains(ch);
}

bool Tokenizer::is_all_space_or_punctuation(const Sentence& text) {
   return stdr::all_of(
      text, [](const auto& ch)-> bool {
         return is_punctuation(ch) || is_space(ch);
      }
   );
}

bool Tokenizer::is_english(char16_t ch) {
   return ch >= u'a' && ch <= u'z'
      || ch >= u'A' && ch <= u'Z';
}

bool Tokenizer::is_number(char16_t ch) {
   return ch >= u'0' && ch <= u'9';
}

bool Tokenizer::is_all_english_or_number(const Sentence& text) {
   return stdr::all_of(
      text, [](const auto& ch)-> bool {
         return is_english(ch) || is_number(ch) || is_space(ch);
      }
   );
}

std::vector<Tokenizer::Word> Tokenizer::single_english_or_number_tokenize(const Sentence& text) {
   std::vector<Word> words(1);
   stdr::for_each(
      text, [&words](const auto& ch)-> void {
         if (is_space(ch)) {
            if (!words.back().empty()) {
               words.emplace_back();
            }
            return;
         }
         words.back().push_back(ch);
      }
   );
   return words;
}

std::float64_t Tokenizer::f_get_char_prob(char16_t ch, CharType type) const {
   if (!m_MarkovModel.has_ep(ch)) {
      return m_DefaultPro[std::to_underlying(type)];
   }
   return m_MarkovModel.get_ep(ch, type) == 0
             ? m_MinPro
             : m_MarkovModel.get_ep(ch, type);
}

std::vector<std::u16string> Tokenizer::f_preprocess(const std::u16string& text) {
   std::vector<std::u16string> result(1);
   bool lastCharIsEngOrNum = false;
   stdr::for_each(
      text,
      [&result, &lastCharIsEngOrNum](char16_t ch)-> void {
         if (is_punctuation(ch)) {
            if (!result.back().empty()) {
               result.emplace_back();
            }
            return;
         }
         const auto isEngOrNum = is_english(ch) || is_number(ch);
         if (!is_space(ch)) {
            if (isEngOrNum != lastCharIsEngOrNum) {
               lastCharIsEngOrNum = isEngOrNum;
               if (!result.back().empty()) {
                  result.emplace_back();
               }
            }
            result.back().push_back(ch);
            return;
         }
         if (!lastCharIsEngOrNum) {
            if (!result.back().empty()) {
               result.emplace_back();
            }
            return;
         }
         if (result.back().empty()) {
            return;
         }
         if (!is_space(result.back().back())) {
            result.back().push_back(ch);
         }
      }
   );
   std::erase_if(
      result, [](const Sentence& sentence)-> bool {
         if (sentence.empty()) {
            return true;
         }
         if (is_all_space_or_punctuation(sentence)) {
            return true;
         }
         return false;
      }
   );
   stdr::for_each(
      result, [](Sentence& sentence) {
         if (is_space(sentence.back())) {
            sentence.pop_back();
         }
      }
   );
   return result;
}

std::vector<Tokenizer::Word> Tokenizer::f_single_chinese_tokenize(const std::u16string& text) {
   std::vector<Word> words;

   CharType lastType = m_MarkovModel.get_isd(CharType::SINGLE) * f_get_char_prob(text.front(), CharType::SINGLE)
                       > m_MarkovModel.get_isd(CharType::BEGIN) * f_get_char_prob(text.front(), CharType::BEGIN)
                          ? CharType::SINGLE
                          : CharType::BEGIN;
   words.push_back(std::u16string{text.front()});

   stdr::for_each(
      text.begin() + 1, text.end(), [this,&words,&lastType](const auto& ch) {
         std::array<std::float64_t, 4> prob;
         for (uint8_t i = 0; i < 4; ++i) {
            auto thisType = static_cast<CharType>(i);
            prob[i]       = f_get_char_prob(ch, thisType)
               * m_MarkovModel.get_tp(lastType, thisType);
         }
         const auto maxIter = stdr::max_element(prob);
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
