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
   std::vector<uint8_t> data=trainer.get_tokenizer_trainer().get_model_data();

   return 0;
}
