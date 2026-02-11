//
// Created by QinBu_Rua on 2026/2/10.
//

#ifndef AUTO_SORTING_MACHINE_READFILESHELPER_H
#define AUTO_SORTING_MACHINE_READFILESHELPER_H

#include <filesystem>
#include <source_location>
#include <string>
#include <vector>

namespace QinBuRua::auto_sorting_machine::details::tokenizer_trainer {

class ReadFilesHelper {
public:
   ReadFilesHelper() = default;
   ReadFilesHelper(const std::string& path);

   void clear();
   void load(const std::string& path, const std::source_location& sl = std::source_location::current());
   void run(const std::source_location& sl = std::source_location::current());
   std::vector<std::u16string>& get_data_ref();

   ReadFilesHelper(ReadFilesHelper&&)                 = delete;
   ReadFilesHelper(const ReadFilesHelper&)            = delete;
   ReadFilesHelper& operator=(ReadFilesHelper&&)      = delete;
   ReadFilesHelper& operator=(const ReadFilesHelper&) = delete;

private:
   std::string m_Path;
   std::vector<std::filesystem::path> m_Files;

   std::vector<std::u16string> m_TrainData;

private:
   void f_read_file(const std::filesystem::path& file_path, const std::source_location& sl);

   void f_initialize(const std::source_location& sl);
   void f_read_files(const std::source_location& sl);
};

}


#endif //AUTO_SORTING_MACHINE_READFILESHELPER_H
