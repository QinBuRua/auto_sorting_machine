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
   WordFrequency wordFrequency;
   auto words = *m_ClassifiedDocuments
      | stdv::values
      | stdv::join
      | stdv::join;
   stdr::for_each(
      words, [&wordFrequency](const Word& word) {
         ++wordFrequency[word];
      }
   );

   const auto& minTf = m_MinTf;
   auto vocabulary   = wordFrequency
      | stdv::filter([minTf](const auto& pair) { return pair.second >= minTf; })
      | stdv::keys
      | stdr::to<Vocabulary>();

   return vocabulary;
}
