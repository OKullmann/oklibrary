// Project: OKSystem Backup
// Author: Tony H. Bao
// Date: 06.04.2005
// Version: 0.2

#include <iostream>
#include <string>

#include "Path_Loader.hpp"
#include "Backup.hpp"
#include "Info_Holder.hpp"
#include "Command_Caller.hpp"
 

int main(const int argc, const char* const argv[]) {
  
  const std::string version_no("0.2");
  const std::string banner = "OKSystem Backup Version " + version_no;
  std::cout << banner << '\n';
     
  if (argc != 3) {
    const std::string error_no_input = "Please specify source and destination directories!";
    const std::string info_usage = "Backup [source] [destination]";
    std::cout << error_no_input << '\n' << info_usage << '\n';
    return 1;
  }

  InfoHolder::DirectoryContainer<std::string> backup_dir;

  const std::string source = argv[1];
  const std::string dest = argv[2];

  try {
    PathLoader::varify_source(source, backup_dir);
    PathLoader::varify_dest(dest, backup_dir);

    std::cout << CommandCaller::pack(backup_dir);
    std::cout << "Successfully Created Backup Archive " << backup_dir.archive() << '\n';
   
    std::cout << CommandCaller::store(backup_dir);
    std::cout << "Successfully Copied Backup Archive " << backup_dir.archive() << " To " << backup_dir.destination() << '\n';
    
    std::cout << CommandCaller::remove(backup_dir);
    
  }
  catch(const Backup::Error::Directory_Error& e){
    std::cerr << e.what() << std::endl;
    return 1;
  }
  catch(const Backup::Error::Command_Error& e){
    std::cerr << e.what() << std::endl;
    return 1;
  }
  catch(...){
    std::cerr << "Program Aborted or Unknown Error Occured!" << std::endl;
    return 1;
  }
  
  const std::string acknowledge = "Backup Done!";
  std::cout << acknowledge << '\n';
  return 0;
  
}
