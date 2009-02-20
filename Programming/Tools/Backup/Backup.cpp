// Tony H. Bao, 6.4.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <string>
#include <cstdlib>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <OKlib/Programming/Tools/Backup/Path_Loader.hpp>
#include <OKlib/Programming/Tools/Backup/Backup.hpp>
#include <OKlib/Programming/Tools/Backup/Info_Holder.hpp>
#include <OKlib/Programming/Tools/Backup/Command_Caller.hpp>

#include <OKlib/General/ErrorHandling.hpp>

// namespace Msg {
  
//   typedef OKlib::Messages::UserInterface::MsgService Service;
//   typedef Service::Messages_base message_type;
//   typedef Service::string_type string_type;
//   typedef Service::Language<OKlib::Messages::UserInterface::deutsch> Deutsch_type;
//   typedef Service::Language<OKlib::Messages::UserInterface::english> English_type;
//   typedef Service::Language<OKlib::Messages::UserInterface::chinese> Chinese_type;
  
//   struct Banner : message_type {
//     const Banner* identity() const {
//       return this;
//     }

//     string_type translate(const Deutsch_type*) const {
//       return "Deutsche Meldung!";
//     }

//     string_type translate(const English_type*) const {
//       return "OKSystem Backup: Compiled on " + std::string(__DATE__) + " at " + std::string(__TIME__);
//     }
    
//     string_type translate(const Chinese_type*) const {
//       return "Chinese message!";
//     }
//   };

// }


int main(const int argc, const char* const argv[]) {

  // ToDo: Using Messages
  
  //const Msg::Banner banner;
    
  using Backup::pfx;
  // ToDo: "pfx" -> expressive
  //const std::string banner = "OKSystem Backup: Compiled on " + std::string(__DATE__) + " at " + std::string(__TIME__);
//   std::cout << Msg::Deutsch_type::ptr() -> name() << '\n';
//   std::cout << Msg::Deutsch_type::ptr() << banner << '\n';
//   std::cout << Msg::Deutsch_type::ptr() << "[Backup 3] " << banner << '\n';
     
  if (argc != 3) {
    int i = 0; std::string call = "";
    while (i < argc) {call = call + std::string(argv[i]) + " "; ++i; }
    const std::string error_no_input = "Usage Error : " + call;
    const std::string info_usage = "Usage : Backup [source] [destination]";
    std::cerr << pfx(error_no_input) << '\n' << pfx(info_usage) << '\n';
    return EXIT_FAILURE;
  }

  InfoHolder::DirectoryContainer<std::string> backup_dir;

  const std::string source = argv[1];
  const std::string dest = argv[2];

  try {
    const std::string startup = "Backup started on " + Backup::DateTime::current_datetime();
    std::cout << startup << '\n';

    PathLoader::verify_source(source, backup_dir);
    PathLoader::verify_dest(dest, backup_dir);

    std::cout << CommandCaller::pack(backup_dir);
    std::cout << "Successfully Created Backup Archive " << backup_dir.archive() << '\n';
   
    std::cout << CommandCaller::store(backup_dir);
    std::cout << "Successfully Copied Backup Archive " << backup_dir.archive() << " To " << backup_dir.destination() << '\n';
    
    std::cout << CommandCaller::remove(backup_dir);
    std::cout << "Successfully Removed Backup Archive " << backup_dir.archive() << '\n';
    
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
  
  const std::string acknowledge = "Backup finished on " + Backup::DateTime::current_datetime();
  std::cout << acknowledge << "\n\n";
}
