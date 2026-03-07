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
   //todo: 需重构,调整主要对外接口为fit,transform,fit_transform
public:
   using Word                = std::u16string;
   using Document            = std::vector<Word>;
   using Documents           = std::vector<Document>;
   using Category            = std::u16string;
   using ClassifiedDocuments = std::unordered_map<Category, Documents>;

   using Vocabulary        = std::unordered_set<Word>;
   using WordToNumTable    = std::unordered_map<Word, uint32_t>;
   using DocumentsWordInfo = std::unordered_map<Word, std::pair<uint32_t/*出现该词的文章数目*/, uint32_t/*该词出现的总数目*/>>;
   using TfVector          = std::vector<std::float32_t>;
   using IdfVector         = std::vector<std::float32_t>;
   using TfidfVector       = std::vector<std::float32_t>;
   using TfidfVectors      = std::vector<TfidfVector>;
   using DocumentsVectors  = std::unordered_map<Category, TfidfVectors>;

   struct Arguments {
      uint32_t& min_word_count;
      uint32_t& min_tf;
      std::float32_t& max_tf;
   };

public:
   void clear();
   void clear_without_arguments();
   Arguments arguments();

   void fit(const ClassifiedDocuments& classified_documents);
   TfidfVector transform(const Document& document);
   DocumentsVectors fit_transform(std::shared_ptr<ClassifiedDocuments> classified_documents);

private:
   uint32_t m_MinWordCount = 1000;
   uint32_t m_MinTf        = 3;
   std::float32_t m_MaxTf  = 0.9f;

   Vocabulary m_Vocabulary;
   WordToNumTable m_WordToNumTable;
   IdfVector m_IdfVector;

private:
   static DocumentsWordInfo f_extract_documents_word_info(const ClassifiedDocuments& classified_documents);
   static WordToNumTable f_make_word_to_num_table(const Vocabulary& vocabulary);
};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFVECTORIZER_H
