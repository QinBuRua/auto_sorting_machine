#include <iostream>
#include <fstream>

#include "classes/CharTypeArray.h"
#include "classes/Trainer.h"
#include "Logger/Logger.h"

using std::cout;
using std::endl;
using namespace QinBuRua::auto_sorting_machine;
using utils::Logger;
using utils::LogLevel;

int main() {
   Logger::set_log_file("logs\\latest.txt");
   Logger::log(LogLevel::INFO, "Begin training");
   Trainer trainer(std::string(R"(data\train\config.json)"));
   trainer.run();
   trainer.get_tokenizer_trainer().write_to_file(R"(data\model\markov_chain_model.dat)");

   return 0;
}
