#include <fstream>
#include <iostream>
#include <ranges>

#include "classes/Tokenizer.h"
#include "classes/TokenizerTrainer.h"
#include "classes/Trainer.h"
#include "details/MarkovChainModel/ParseModelHelper.h"
#include "details/ModelHeader/ParseHeaderHelper.h"
#include "Logger/Logger.h"

namespace stdr = std::ranges;
using std::cout;
using std::endl;
using namespace QinBuRua::auto_sorting_machine;
namespace slog = utils::log;
using utils::LogLevel;
using details::model_header::ParseHeaderHelper;
using details::markov_chain_model::ParseModelHelper;


int main() {
   utils::Logger::instance().set_log_level(LogLevel::DEBUG);
   slog::info_sl("Begin training");
   Trainer trainer(std::string(R"(data\train\config.json)"));
   trainer.run();
   auto& tokenizerTrainer = trainer.get_tokenizer_trainer();

   std::vector<std::byte> rawData = tokenizerTrainer.get_model_data();
   std::ofstream outFile{R"(data\model\MCM.dat)", std::ios::binary};
   outFile.write(reinterpret_cast<const char*>(rawData.data()), rawData.size());
   ParseHeaderHelper parseHeaderHelper{rawData.begin()};
   parseHeaderHelper.run();
   auto header = std::move(parseHeaderHelper.get_header_ref());

   ParseModelHelper parseModelHelper{parseHeaderHelper.get_iter()};
   parseModelHelper.run();
   auto model = std::move(parseModelHelper.get_model_ref());

   Tokenizer tokenizer{std::move(model)};
   tokenizer.initialize();
   auto words = tokenizer.tokenize(
      u"关于 “AI” 的 讨论：有人说 它比人强 （例如在 围棋上，AlphaGo 4:1 战胜了 李世石），但也 有人说 它很 “蠢”。你 怎么看 呢？欢迎 在 评论区 留言！网址：https：//www. aisys. net/zh-cn/faq ？ q=test & page=1 邮箱： ai-talk @ futura. ai 电话： +86 (010) 1234 - 5678"
   );
   tokenizer.load_from_file(R"(data\model\MCM.dat)");

   return 0;
}
