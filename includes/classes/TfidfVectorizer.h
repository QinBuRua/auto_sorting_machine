//
// Created by QinBu_Rua on 2026/2/25.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFVECTORIZER_H
#define AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFVECTORIZER_H

#include <memory>
#include <stdfloat>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace QinBuRua::auto_sorting_machine {

class TfidfVectorizer {
   //todo:未完成的类
public:
   using Word                = std::u16string;
   using Document            = std::vector<Word>;
   using Documents           = std::vector<Document>;
   using Category            = std::u16string;
   using ClassifiedDocuments = std::unordered_map<Category, Documents>;

   using Vocabulary          = std::unordered_set<Word>;
   using WordToNumTable      = std::unordered_map<Word, uint32_t>;
   using RawVector           = std::vector<uint32_t>;
   using WordCount           = std::unordered_map<Word, uint32_t>;
   using TfVector            = std::vector<std::float32_t>;
   using TfVectors           = std::vector<TfVector>;
   using ClassifiedTfVectors = std::unordered_map<Category, TfVectors>;
   using IdfVector           = std::vector<std::float32_t>;
   using TfidfVector         = std::vector<std::float32_t>;
   using TfidfVectors        = std::vector<TfidfVector>;
   using DocumentsVectors    = std::unordered_set<Category, TfidfVectors>;

   struct Arguments {
      uint32_t& min_word_count;
      uint32_t& min_tf;
      std::float32_t& max_tf;
   };

public:
   TfidfVectorizer() = default;
   explicit TfidfVectorizer(std::shared_ptr<ClassifiedDocuments> documents);

   void clear();
   void load(std::shared_ptr<ClassifiedDocuments> documents);
   Arguments arguments();
   void run();

private:
   uint32_t m_MinWordCount = 1000;
   uint32_t m_MinTf        = 2;
   std::float32_t m_MaxTf  = 0.9f;

   std::shared_ptr<const ClassifiedDocuments> m_ClassifiedDocuments;
   Vocabulary m_Vocabulary;
   WordToNumTable m_WordToNumTable;
   std::shared_ptr<DocumentsVectors> m_DocumentsVectors;

private:
   Vocabulary f_extract_vocabulary() const;
   WordToNumTable f_make_word_to_num_table(const Vocabulary& vocabulary) const;
   Vocabulary f_filter_above_min_tf() const;
   Vocabulary f_filter_under_max_tf(const Vocabulary& vocabulary) const;
   RawVector f_calculate_raw_vector(const Document& document) const;
   static TfVector f_calculate_tf_vector(const RawVector& raw_vector);

};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFVECTORIZER_H
