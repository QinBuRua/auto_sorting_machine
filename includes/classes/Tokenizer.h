//
// Created by QinBu_Rua on 2026/2/7.
//

#ifndef AUTO_SORTING_MACHINE_TOKENIZER_H
#define AUTO_SORTING_MACHINE_TOKENIZER_H

#include <vector>
#include <string>

#include "MarkovChainModel.h"

namespace QinBuRua::auto_sorting_machine{

class Tokenizer {
public:

private:
   MarkovChainModel m_MarkovModel;
   std::wstring m_RawSentence;
   std::vector<std::wstring> m_Words;
};

}


#endif //AUTO_SORTING_MACHINE_TOKENIZER_H