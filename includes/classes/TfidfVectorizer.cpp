//
// Created by QinBu_Rua on 2026/2/25.
//

#include <cmath>
#include <algorithm>
#include <ranges>

#include "TfidfVectorizer.h"
#include "Logger/Logger.h"

namespace stdr = std::ranges;
namespace stdv = std::views;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;

TfidfVectorizer::TfidfVectorizer(std::shared_ptr<ClassifiedDocuments> documents) {
   uint32_t wordCounts = stdr::distance(
      *documents
      | stdv::values
      | stdv::join
      | stdv::join
   );
   if (wordCounts < m_MinWordCount) {
      slog::warn(std::format(
         "From TfidfVectorizer: Dataset is too small, totally {} words less than {} words",
         wordCounts, m_MinWordCount
      ));
   }
   m_ClassifiedDocuments = std::move(documents);
}

void TfidfVectorizer::clear() {
   m_MinWordCount = 1000;
   m_MinTf        = 3;
   m_MaxTf        = 0.9f;

   m_ClassifiedDocuments = nullptr;
   m_Vocabulary.clear();
   m_WordToNumTable.clear();
   m_DocumentsVectors = nullptr;
}

void TfidfVectorizer::load(std::shared_ptr<ClassifiedDocuments> documents) {
   uint32_t wordCounts = stdr::distance(
      *documents
      | stdv::values
      | stdv::join
      | stdv::join
   );
   if (wordCounts < m_MinWordCount) {
      slog::warn(
         std::format(
            "From TfidfVectorizer: Dataset is too small, totally {} words less than {} words", wordCounts,
            m_MinWordCount
         )
      );
   }
   clear();
   m_ClassifiedDocuments = std::move(documents);
}

TfidfVectorizer::Arguments TfidfVectorizer::arguments() {
   return Arguments{.min_word_count = m_MinWordCount, .min_tf = m_MinTf, .max_tf = m_MaxTf};
}

void TfidfVectorizer::run() {
   m_Vocabulary = f_extract_vocabulary();
   if (m_Vocabulary.empty()) {
      slog::warn_throw<std::runtime_error>(
         "From TfidfVectorizer: Vocabulary is empty. Please check the min/max tf and ths size of dataset."
      );
   }

   m_WordToNumTable = f_make_word_to_num_table(m_Vocabulary);
   f_calculate_tf_from_all_documents();
   if (m_ClassifiedTfVectors.empty()) {
      slog::warn_throw<std::runtime_error>(
         "From TfidfVectorizer: Classified TfVectors is empty."
      );
   }

   m_IdfVector = f_calculate_idf_from_all_tf();
   f_calculate_tfidf_vectors();

   f_release_memory();
}

TfidfVectorizer::Vocabulary TfidfVectorizer::f_extract_vocabulary() const {
   const Vocabulary vocabulary = f_filter_above_min_tf();
   return f_filter_under_max_tf(vocabulary);
}

TfidfVectorizer::WordToNumTable TfidfVectorizer::f_make_word_to_num_table(const Vocabulary& vocabulary) {
   WordToNumTable table{vocabulary.size()};
   auto sortedVocabulary = vocabulary
      | stdr::to<std::vector<Word>>();
   stdr::sort(sortedVocabulary);
   for (uint32_t i = 0; i < sortedVocabulary.size(); ++i) {
      table[sortedVocabulary[i]] = i;
   }
   return table;
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
      | stdv::filter([minTf](const auto& pair) {
         return pair.second >= minTf;
      })
      | stdv::keys
      | stdr::to<Vocabulary>();
}

TfidfVectorizer::Vocabulary TfidfVectorizer::f_filter_under_max_tf(const Vocabulary& vocabulary) const {
   WordCount wordCount;
   const auto filteredDocuments = *m_ClassifiedDocuments
      | stdv::values
      | stdv::join
      | stdv::transform([&vocabulary](const auto& document) {
         return document
            | stdv::filter([&vocabulary](const Word& word) { return vocabulary.contains(word); });
      });
   uint32_t documentsCount{0};
   for (auto document : filteredDocuments) {
      ++documentsCount;
      Vocabulary tmpVocabulary;
      for (const auto& word : document) {
         if (tmpVocabulary.contains(word)) {
            continue;
         }
         tmpVocabulary.insert(word);
         ++wordCount[word];
      }
   }
   const auto& maxTf = m_MaxTf;
   return wordCount
      | stdv::filter([maxTf,documentsCount](const auto& pair)-> bool {
         const std::float32_t wordFrequency
            = static_cast<std::float32_t>(pair.second) / static_cast<std::float32_t>(documentsCount);
         return wordFrequency <= maxTf;
      })
      | stdv::keys
      | stdr::to<Vocabulary>();
}

TfidfVectorizer::RawVector TfidfVectorizer::f_calculate_raw_vector(const Document& document) const {
   RawVector rawVector(m_WordToNumTable.size());
   for (const auto& word : document) {
      auto indexIter = m_WordToNumTable.find(word);
      if (indexIter == m_WordToNumTable.end()) {
         continue;
      }
      ++rawVector[indexIter->second];
   }
   return rawVector;
}

TfidfVectorizer::TfVector TfidfVectorizer::f_calculate_tf_vector(const RawVector& raw_vector) {
   auto totalWordCount = static_cast<std::float32_t>(stdr::fold_left(raw_vector, 0, std::plus()));
   return raw_vector
      | stdv::transform([totalWordCount](auto val)-> std::float32_t { return val / totalWordCount; })
      | stdr::to<TfVector>();
}

void TfidfVectorizer::f_calculate_tf_from_all_documents() {
   for (const auto& [category, documents] : *m_ClassifiedDocuments) {
      TfidfVectors tmpTfVectors{};
      for (const auto& document : documents) {
         RawVector tmpRawVector = f_calculate_raw_vector(document);
         tmpTfVectors.push_back(f_calculate_tf_vector(tmpRawVector));
      }
      m_ClassifiedTfVectors[category] = tmpTfVectors;
   }
}

TfidfVectorizer::IdfVector TfidfVectorizer::f_calculate_idf_from_all_tf() {
   const auto flattenedTfVectors = m_ClassifiedTfVectors
      | stdv::values
      | stdv::join;

   IdfVector idfVector(m_WordToNumTable.size());
   for (const auto& tfVector : flattenedTfVectors) {
      for (const auto& [index,value] : tfVector | stdv::enumerate) {
         if (value > 0) {
            ++idfVector[index];
         }
      }
   }

   auto totalDocumentsCount = static_cast<std::float32_t>(stdr::distance(flattenedTfVectors));
   stdr::transform(idfVector, idfVector.begin(), [&totalDocumentsCount](const auto& value) {
      return static_cast<std::float32_t>(std::log2(totalDocumentsCount / value));
   });

   return idfVector;
}

void TfidfVectorizer::f_calculate_tfidf_vectors() {
   m_DocumentsVectors = std::make_shared<DocumentsVectors>(m_ClassifiedTfVectors.size());
   for (const auto& [category, documents] : m_ClassifiedTfVectors) {
      auto& tfidfVectors = (*m_DocumentsVectors)[category];
      tfidfVectors.reserve(documents.size());
      for (const auto& document : documents) {
         auto tfidfVector = stdv::zip_transform(std::multiplies(), document, m_IdfVector)
            | stdr::to<TfidfVector>();
         tfidfVectors.push_back(std::move(tfidfVector));
      }
   }
}

void TfidfVectorizer::f_release_memory() {
   m_ClassifiedDocuments = nullptr;
   m_WordToNumTable.clear();
   m_ClassifiedTfVectors.clear();
}
