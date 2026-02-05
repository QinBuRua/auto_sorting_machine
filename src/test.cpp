#include <iostream>

#include "classes/CharTypeArray.h"
#include "classes/Trainer.h"
#include "classes/ModelHeader.h"

using std::cout;
using std::endl;

using namespace QinBuRua::auto_sorting_machine;

int main() {
   Trainer trainer(std::string(R"(data\train\config.json)"));

   trainer.run();

   return 0;
}
