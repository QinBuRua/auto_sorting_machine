#include <iostream>
#include <fstream>

#include "classes/CharTypeArray.h"
#include "classes/Trainer.h"
#include "Logger/Logger.h"

using std::cout;
using std::endl;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;
using utils::LogLevel;

int main() {
   slog::info("Begin training");
   Trainer trainer(std::string(R"(data\train\config.json)"));
   trainer.run();
   trainer.get_tokenizer_trainer().write_to_file(R"(data\model\markov_chain_model.dat)");

   slog::log_throw<std::runtime_error>(
      slog::Tag{},
      "Missing"
   );

   return 0;
}
