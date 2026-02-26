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
#include <valarray>
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

   using Vocabulary       = std::unordered_set<Word>;
   using RawVector        = std::valarray<uint32_t>;
   using WordFrequency    = std::unordered_map<Word, uint32_t>;
   using TfVector         = std::valarray<std::float32_t>;
   using IdfVector        = std::valarray<std::float32_t>;
   using TfidfVector      = std::valarray<std::float32_t>;
   using TfidfVectors     = std::vector<TfidfVector>;
   using DocumentsVectors = std::unordered_set<Category, TfidfVectors>;

public:

private:

};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFVECTORIZER_H
