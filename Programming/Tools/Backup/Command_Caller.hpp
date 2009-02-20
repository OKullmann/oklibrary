// Tony Bao, 6.4.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#ifndef COMMAND_CALLER_BACKUP_JHnytfa82aslkj
#define COMMAND_CALLER_BACKUP_JHnytfa82aslkj 

#include <string>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <OKlib/General/SystemHandling.hpp>
#include <OKlib/Programming/Tools/Backup/Info_Holder.hpp>
#include <OKlib/Programming/Tools/Backup/Path_Loader.hpp>

namespace CommandCaller {

  // ToDO: Make a LIBRARY out of it.

  std::string pack(InfoHolder::DirectoryContainer<std::string>& backup_dir) {
    
    const std::string find_options = " -follow -depth -print ";
    const std::string cpio_options = " cpio --verbose -o -a -O ";
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    const std::string archive_name = "OKBackup_" + to_iso_string(now) + ".cpio";
    backup_dir.archive(archive_name);
    const std::string command = "find " + backup_dir.source() + find_options + '|' + cpio_options + archive_name;
    
    std::cout << command << '\n';
    SystemHandling::System_bash S;
    SystemHandling::SystemOutput Out = S(command);
    if (not bool(Out)) throw Backup::Error::Command_Error("Unable to create archive file!");
    std::string on_stdout = Out;
    return on_stdout;
  }
  
  std::string store(const InfoHolder::DirectoryContainer<std::string>& backup_dir) {
    SystemHandling::System_bash S;
    const std::string scp_options = "-p -q ";
    const std::string command = "scp " + scp_options + backup_dir.archive() + " " + backup_dir.destination();
    std::string on_stdout;

    if (PathLoader::is_remote(backup_dir.destination())) {
      const std::string fulldir = backup_dir.destination();
      std::string host = "";
      std::string path = "";
      
      PathLoader::strip(fulldir, host, path);
      
      const std::string make_dir = "ssh " + host + " " + "\"mkdir " + path + "PreviousBackup/\"";
      std::cout << make_dir << '\n';
      SystemHandling::SystemOutput makedir = S(make_dir);
      if (not bool(makedir)) throw Backup::Error::Command_Error("Unable to write to destination directory!");
 
      const std::string move_file = "ssh " + host + " " + "\"mv " + path + "OKBackup_*.cpio " + path + "PreviousBackup/\"";
      std::cout << move_file << '\n';
      SystemHandling::SystemOutput movefile = S(move_file);
      if (not bool(movefile)) std::cout << "No previous backup copies found in destination directory." << '\n';
      
      std::cout << command << '\n';
      SystemHandling::SystemOutput Out = S(command);
      if (not bool(Out)) throw Backup::Error::Command_Error("Unable to copy archive file to destination directory!");
      std::string on_stdout = Out;
    
      const std::string remove_previous = "ssh " + host + " " + "\"rm -rf " + path + "PreviousBackup/\"";
      std::cout << remove_previous << '\n';
      SystemHandling::SystemOutput removeprevious = S(remove_previous);
      if (not bool(removeprevious)) throw Backup::Error::Command_Error("Unable to remove previous backup copies from destination directory!");

      return on_stdout;
    }
    
    else {
      const std::string make_dir = "mkdir "+ backup_dir.destination() + "/PreviousBackup/";
      std::cout << make_dir << '\n';
      SystemHandling::SystemOutput makedir = S(make_dir);
      if (not bool(makedir)) throw Backup::Error::Command_Error("Unable to write to destination directory!");

      const std::string move_file = "mv " + backup_dir.destination() + "/OKBackup_*.cpio " + backup_dir.destination() + "/PreviousBackup/";
      std::cout << move_file << '\n';
      SystemHandling::SystemOutput movefile = S(move_file);
      if (not bool(movefile)) std::cout << "No previous backup copies found in destination directory." << '\n';

      std::cout << command << '\n';    
      SystemHandling::SystemOutput Out = S(command);
      if (not bool(Out)) throw Backup::Error::Command_Error("Unable to copy archive file to destination directory!");
      std::string on_stdout = Out;
    
      const std::string remove_previous = "rm -rf " + backup_dir.destination() + "/PreviousBackup/";
      std::cout << remove_previous << '\n';
      SystemHandling::SystemOutput removeprevious = S(remove_previous);
      if (not bool(removeprevious)) throw Backup::Error::Command_Error("Unable to remove previous backup copies from destination directory!");

      return on_stdout;
    }
    
    return on_stdout;
  }

  std::string remove(const InfoHolder::DirectoryContainer<std::string>& backup_dir) {

    const std::string rm_options = "-f ";

    const std::string command = "rm " + rm_options + backup_dir.archive();
 
    std::cout << command << '\n';
    SystemHandling::System_bash S;
    SystemHandling::SystemOutput Out = S(command);
    if (not bool(Out)) throw Backup::Error::Command_Error("Unable to remove archive file!");
    std::string on_stdout = Out;
    return on_stdout;
  }
}

#endif
