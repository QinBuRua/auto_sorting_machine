//
// Created by QinBu_Rua on 2026/2/25.
//

#include <algorithm>
#include <cassert>
#include <cmath>
#include <ranges>

#include "TfidfVectorizer.h"
#include "Logger/Logger.h"

namespace stdr = std::ranges;
namespace stdv = std::views;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;

void TfidfVectorizer::clear() {
   m_MinWordCount = 1000;
   m_MinTf        = 3;
   m_MaxTf        = 0.9f;

   m_Vocabulary.clear();
   m_WordToNumTable.clear();
   IdfVector{}.swap(m_IdfVector);
}

void TfidfVectorizer::clear_without_arguments() {
   m_Vocabulary.clear();
   m_WordToNumTable.clear();
   IdfVector{}.swap(m_IdfVector);
}

TfidfVectorizer::Arguments TfidfVectorizer::arguments() {
   return Arguments{.min_word_count = m_MinWordCount, .min_tf = m_MinTf, .max_tf = m_MaxTf};
}

void TfidfVectorizer::fit(const ClassifiedDocuments& classified_documents) {
   clear_without_arguments();

   const auto flattenedDocuments = classified_documents
      | stdv::values
      | stdv::join;
   const auto documentCount  = stdr::distance(flattenedDocuments);
   const auto totalWordCount = stdr::distance(flattenedDocuments | stdv::join);
   if (totalWordCount < m_MinWordCount) {
      slog::warn(std::format("From TfidfVectorizer: The dataset is too small, only {} words, less than {} words",
         totalWordCount, m_MinWordCount));
   }

   const auto documentCountFloat = static_cast<std::float32_t>(documentCount);

   DocumentsWordInfo documentsWordInfo = f_extract_documents_word_info(classified_documents);

   const auto& minTf              = m_MinTf;
   const auto& maxTf              = m_MaxTf;
   auto filteredDocumentsWordInfo = documentsWordInfo
      | stdv::filter([&documentCountFloat,&minTf,&maxTf](const auto& pair) {
         if (pair.second.second < minTf) {
            return false;
         }
         const auto wordFrequency
            = static_cast<std::float32_t>(pair.second.first) / documentCountFloat;
         return wordFrequency <= maxTf;
      });

   m_Vocabulary = filteredDocumentsWordInfo
      | stdv::keys
      | stdr::to<Vocabulary>();
   m_WordToNumTable = f_make_word_to_num_table(m_Vocabulary);

   m_IdfVector.resize(m_Vocabulary.size());
   for (const auto& [word, info] : filteredDocumentsWordInfo) {
      const auto frequency = documentCountFloat / static_cast<std::float32_t>(info.first);
      m_IdfVector[m_WordToNumTable[word]]
         = static_cast<std::float32_t>(std::log2(static_cast<double>(frequency)));
   }
}

TfidfVectorizer::DocumentsWordInfo TfidfVectorizer::f_extract_documents_word_info(
   const ClassifiedDocuments& classified_documents
) {
   const auto flattenedDocuments = classified_documents
      | stdv::values
      | stdv::join;

   DocumentsWordInfo documentsWordInfo;
   for (const auto& document : flattenedDocuments) {
      Vocabulary tmpVocabulary;

      for (const auto& word : document) {
         auto& [documentCount, wordCount] = documentsWordInfo[word];
         ++wordCount;

         if (tmpVocabulary.contains(word)) {
            continue;
         }
         tmpVocabulary.insert(word);
         ++documentCount;
      }
   }

   return documentsWordInfo;
}

TfidfVectorizer::WordToNumTable TfidfVectorizer::f_make_word_to_num_table(const Vocabulary& vocabulary) {
   auto words = vocabulary | stdr::to<std::vector<Word>>();
   stdr::sort(words);

   WordToNumTable wordToNumTable{words.size()};
   for (const auto& [index, word] : words | stdv::enumerate) {
      wordToNumTable[word] = index;
   }

   return wordToNumTable;
}
