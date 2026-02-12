//
// Created by QinBu_Rua on 2026/2/7.
//

#ifndef AUTO_SORTING_MACHINE_TOKENIZERTRAINER_H
#define AUTO_SORTING_MACHINE_TOKENIZERTRAINER_H

#include <string>
#include <vector>

#include "MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine {

class Tokenizer {
public:
   explicit Tokenizer(const MarkovChainModel& model);
   explicit Tokenizer(MarkovChainModel&& model) noexcept;

   void load_from_file(const std::string& filename); //todo

private:
   MarkovChainModel m_MarkovModel;
   std::wstring m_RawSentence;
   std::vector<std::wstring> m_Words;
};

}


#endif //AUTO_SORTING_MACHINE_TOKENIZER_H
