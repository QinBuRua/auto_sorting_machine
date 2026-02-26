//
// Created by QinBu_Rua on 2026/2/25.
//

#ifndef AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFTRAINER_H
#define AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFTRAINER_H

#include <memory>
#include <stdfloat>
#include <string>
#include <unordered_map>
#include <vector>

namespace QinBuRua::auto_sorting_machine {

class TfidfTrainer {
public:
   using Word               = std::u16string;
   using Document           = std::vector<Word>;
   using Documents          = std::vector<Document>;
   using Category           = std::u16string;
   using ClassifiedDocument = std::unordered_map<Category, Documents>;

public:
   TfidfTrainer() = default;
   TfidfTrainer(std::shared_ptr<ClassifiedDocument> train_data);
   TfidfTrainer(std::shared_ptr<const ClassifiedDocument> train_data);

   void load(std::shared_ptr<ClassifiedDocument> train_data) noexcept;
   void load(std::shared_ptr<const ClassifiedDocument> train_data) noexcept;

private:
   std::shared_ptr<const std::unordered_map<Category, Documents>> m_Tokens;
   uint32_t m_MinDf       = 3;
   std::float64_t m_MaxDf = 0.9;
   std::shared_ptr<std::valarray<std::float64_t>> m_Vector;

};

}

#endif //AUTO_SORTING_MACHINE_INCLUDES_CLASSES_TFIDFTRAINER_H
