//
// Created by QinBu_Rua on 2026/2/25.
//

#include "TfidfTrainer.h"

using namespace QinBuRua::auto_sorting_machine;

TfidfTrainer::TfidfTrainer(std::shared_ptr<std::unordered_map<Category, Documents>> train_data) {
   m_Tokens = std::move(train_data);
}

TfidfTrainer::TfidfTrainer(std::shared_ptr<const ClassifiedDocument> train_data) {
   m_Tokens = std::move(train_data);
}

void TfidfTrainer::load(std::shared_ptr<std::unordered_map<Category, Documents>> train_data) noexcept {
   m_Tokens = std::move(train_data);
}

void TfidfTrainer::load(std::shared_ptr<const std::unordered_map<Category, Documents>> train_data) noexcept {
   m_Tokens = std::move(train_data);
}
