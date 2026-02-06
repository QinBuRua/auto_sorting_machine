#include <iostream>
#include <fstream>

#include "classes/CharTypeArray.h"
#include "classes/Trainer.h"

using std::cout;
using std::endl;

using namespace QinBuRua::auto_sorting_machine;

int main() {
   Trainer trainer(std::string(R"(data\train\config.json)"));
   trainer.run();
   trainer.get_tokenizer_trainer().write_to_file(R"(data\model\markov_chain_model.dat)");

   return 0;
}
