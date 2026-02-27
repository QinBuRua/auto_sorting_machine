//
// Created by QinBu_Rua on 2026/2/25.
//

#include <ranges>

#include "TfidfVectorizer.h"

namespace stdr = std::ranges;
namespace stdv = std::views;
using namespace QinBuRua::auto_sorting_machine;

TfidfVectorizer::TfidfVectorizer(std::shared_ptr<ClassifiedDocuments> documents) {
   m_ClassifiedDocuments = std::move(documents);
}

void TfidfVectorizer::clear() {
   m_MinTf = 3;

   m_ClassifiedDocuments = nullptr;
   m_Vocabulary.clear();
   m_DocumentsVectors.clear();
}

void TfidfVectorizer::load(std::shared_ptr<ClassifiedDocuments> documents) {
   clear();
   m_ClassifiedDocuments = std::move(documents);
}

void TfidfVectorizer::run() {
   m_Vocabulary = f_extract_vocabulary();
}

TfidfVectorizer::Vocabulary TfidfVectorizer::f_extract_vocabulary() const {
   const Vocabulary vocabulary = f_filter_above_min_tf();
   return f_filter_under_max_tf(vocabulary);
}

TfidfVectorizer::Vocabulary TfidfVectorizer::f_filter_above_min_tf() const {
   WordCount wordCount;
   auto words = *m_ClassifiedDocuments
      | stdv::values
      | stdv::join
      | stdv::join;
   stdr::for_each(
      words, [&wordCount](const Word& word) {
         ++wordCount[word];
      }
   );
   const auto& minTf = m_MinTf;
   return wordCount
      | stdv::filter([minTf](const auto& pair) { return pair.second >= minTf; })
      | stdv::keys
      | stdr::to<Vocabulary>();
}

TfidfVectorizer::Vocabulary TfidfVectorizer::f_filter_under_max_tf(const Vocabulary& vocabulary) const {
   WordCount wordCount;
   const auto filteredDocuments = *m_ClassifiedDocuments
      | stdv::values
      | stdv::join
      | stdv::transform(
         [&vocabulary](const auto& document) {
            return document
               | stdv::filter([&vocabulary](const Word& word) { return vocabulary.contains(word); });
         }
      );
   uint32_t documentsCount{0};
   for (auto document: filteredDocuments) {
      ++documentsCount;
      Vocabulary tmpVocabulary;
      for (const auto& word: document) {
         if (tmpVocabulary.contains(word)) {
            continue;
         }
         tmpVocabulary.insert(word);
         ++wordCount[word];
      }
   }
   const auto& maxTf = m_MaxTf;
   return wordCount
      | stdv::filter(
         [maxTf,documentsCount](const auto& pair)-> bool {
            const std::float32_t wordFrequency
               = static_cast<std::float32_t>(pair.second) / static_cast<std::float32_t>(documentsCount);
            return wordFrequency <= maxTf;
         }
      )
      | stdv::keys
      | stdr::to<Vocabulary>();
}
