// Project: OKSystem Backup
// Author: Tony H. Bao
// Date: 06.04.2005
// Version: 0.2

#include <iostream>
#include <string>
#include <cstdlib>

#include "Path_Loader.hpp"
#include "Backup.hpp"
#include "Info_Holder.hpp"
#include "Command_Caller.hpp"
 

int main(const int argc, const char* const argv[]) {

  // ToDo: Using Messages
  
  const std::string banner = "OKSystem Backup";
  // ToDo: compilation time and date
  // ToDo: current date and time
  // ToDo: Output identifier
  std::cout << banner << '\n';
     
  // ToDo: string variable "program identification"
  // Every output prefixed by it.

  if (argc != 3) {
    const std::string error_no_input = "Please specify source and destination directories!";
    // ToDo: Output the parameters
    const std::string info_usage = "Backup [source] [destination]";
    std::cerr << error_no_input << '\n' << info_usage << '\n';
    return EXIT_FAILURE;
  }

  InfoHolder::DirectoryContainer<std::string> backup_dir;

  const std::string source = argv[1];
  const std::string dest = argv[2];

  try {
    // ToDO: "verify"
    PathLoader::varify_source(source, backup_dir);
    PathLoader::varify_dest(dest, backup_dir);

    std::cout << CommandCaller::pack(backup_dir);
    std::cout << "Successfully Created Backup Archive " << backup_dir.archive() << '\n';
   
    std::cout << CommandCaller::store(backup_dir);
    std::cout << "Successfully Copied Backup Archive " << backup_dir.archive() << " To " << backup_dir.destination() << '\n';
    
    std::cout << CommandCaller::remove(backup_dir);
    // ToDo: make "store" and "remove" follow the same principle
    
  }
  catch(const Backup::Error::Directory_Error& e){
    // ToDo: more information (using general library)
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch(const Backup::Error::Command_Error& e){
    // ToDo: more information (using general library)
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }
  catch(...){
    std::cerr << "Program Aborted or Unknown Error Occured!" << "\n";
    return EXIT_FAILURE;
  }
  
  const std::string acknowledge = "Backup Done!";
  std::cout << acknowledge << '\n\n';
  // ToDo: time and date
  
}
