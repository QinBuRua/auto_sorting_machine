//
// Created by QinBu_Rua on 2026/2/7.
//

#include "Tokenizer.h"
#include "details/Tokenizer/ReadModelHelper.h"

using namespace QinBuRua::auto_sorting_machine;

Tokenizer::Tokenizer(const MarkovChainModel& model) {
   m_MarkovModel = model;
}

Tokenizer::Tokenizer(MarkovChainModel&& model) noexcept {
   m_MarkovModel = std::move(model);
}

void Tokenizer::load_from_file(const std::string& filename) {
   using details::tokenizer::ReadModelHelper;
   ReadModelHelper helper;

}
