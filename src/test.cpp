#include <fstream>
#include <iostream>
#include <ranges>

#include "classes/TfidfVectorizer.h"
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
   // utils::Logger::instance().set_log_level(LogLevel::DEBUG);
   // slog::info_sl("Begin training");
   // Trainer trainer(std::string(R"(data\train\config.json)"));
   // trainer.run();
   // auto& tokenizerTrainer = trainer.get_tokenizer_trainer();
   //
   // std::vector<std::byte> rawData = tokenizerTrainer.get_model_data();
   // std::ofstream outFile{R"(data\model\MCM.dat)", std::ios::binary};
   // outFile.write(reinterpret_cast<const char*>(rawData.data()), rawData.size());
   // ParseHeaderHelper parseHeaderHelper{rawData.begin()};
   // parseHeaderHelper.run();
   // auto header = std::move(parseHeaderHelper.get_header_ref());
   //
   // ParseModelHelper parseModelHelper{parseHeaderHelper.get_iter()};
   // parseModelHelper.run();
   // auto model = std::move(parseModelHelper.get_model_ref());
   //
   // Tokenizer tokenizer{std::move(model)};
   // tokenizer.initialize();
   // auto words = tokenizer.tokenize(
   //    u"关于 “AI” 的 讨论：有人说 它比人强 （例如在 围棋上，AlphaGo 4:1 战胜了 李世石），但也 有人说 它很 “蠢”。你 怎么看 呢？欢迎 在 评论区 留言！网址：https：//www. aisys. net/zh-cn/faq ？ q=test & page=1 邮箱： ai-talk @ futura. ai 电话： +86 (010) 1234 - 5678"
   // );
   // tokenizer.load_from_file(R"(data\model\MCM.dat)");

   auto data = std::make_shared<TfidfVectorizer::ClassifiedDocuments>(
      TfidfVectorizer::ClassifiedDocuments{
         {
            u"科技", {
               {
                  // 科技文档 1：计算机相关，含重复词
                  u"计算机", u"编程", u"算法", u"数据结构", u"操作系统",
                  u"网络", u"数据库", u"计算机", u"编程", u"算法"
               },
               {
                  // 科技文档 2：人工智能相关，含重复词
                  u"人工智能", u"机器学习", u"深度学习", u"神经网络", u"自然语言",
                  u"计算机视觉", u"人工智能", u"机器学习", u"深度学习", u"神经网络"
               }
            }
         },
         {
            u"体育", {
               {
                  // 体育文档 1：球类运动，含重复词
                  u"足球", u"篮球", u"排球", u"乒乓球", u"羽毛球",
                  u"网球", u"足球", u"篮球", u"排球", u"乒乓球"
               },
               {
                  // 体育文档 2：赛事相关，含重复词
                  u"奥运会", u"世界杯", u"锦标赛", u"冠军", u"亚军",
                  u"季军", u"奥运会", u"世界杯", u"锦标赛", u"冠军"
               }
            }
         }
      }
   );

   TfidfVectorizer vectorizer{data};
   vectorizer.run();

   return 0;
}
