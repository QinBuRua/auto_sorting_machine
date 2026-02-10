#include <fstream>
#include <iostream>

#include "classes/CharTypeArray.h"
#include "classes/Trainer.h"
#include "details/MarkovChainModel/ParseModelHelper.h"
#include "details/ModelHeader/ParseHeaderHelper.h"
#include "Logger/Logger.h"

using std::cout;
using std::endl;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;
using utils::LogLevel;
using details::model_header::ParseHeaderHelper;
using details::markov_chain_model::ParseModelHelper;


int main() {
   slog::info("Begin training");
   Trainer trainer(std::string(R"(data\train\config.json)"));
   trainer.run();
   auto& tokenizerTrainer = trainer.get_tokenizer_trainer();

   std::vector<uint8_t> rawData = tokenizerTrainer.get_model_data();
   ParseHeaderHelper parseHeaderHelper{rawData.begin()};
   parseHeaderHelper.run();
   auto header = std::move(parseHeaderHelper.get_header_ref());

   ParseModelHelper parseModelHelper{parseHeaderHelper.get_iter()};
   parseModelHelper.run();
   auto model = std::move(parseModelHelper.get_model_ref());

   return 0;
}
