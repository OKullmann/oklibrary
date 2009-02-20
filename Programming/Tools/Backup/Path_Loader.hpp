// Tony Bao, 6.4.2007 (Swansea)
/* Copyright 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Path_Loader.hpp
  \brief Needs a complete overhaul.

  \todo Written by TB; untested (and likely with errors).
*/

#ifndef PATH_LOADER_BACKUP_tjyahsaggi876qw93
#define PATH_LOADER_BACKUP_tjyahsaggi876qw93

#include <string>
#include <cassert>

#include <boost/filesystem/operations.hpp>

#include <OKlib/Programming/Tools/Backup/Info_Holder.hpp>
#include <OKlib/Programming/Tools/Backup/Backup.hpp>

namespace PathLoader {

  // ToDO: Making a LIBRARY out of it.
  
  namespace fs = boost::filesystem;

  typedef InfoHolder::DirectoryContainer<std::string> container;

  void verify_source(const std::string& cmd_source, container& backup_dir) {
    fs::path source_dir(fs::initial_path());
    source_dir = fs::system_complete(fs::path(cmd_source, fs::native));
    backup_dir.source(source_dir.native_file_string());
    if (not fs::exists(source_dir)) throw Backup::Error::Directory_Error("Source directory does not exist!");      
    if (not fs::is_directory(source_dir)) throw Backup::Error::Directory_Error("Source specified is not a directory!");
    if (fs::is_empty(source_dir)) throw Backup::Error::Directory_Error("Source directory is empty!");
  }
  
  const bool is_remote(const std::string& dest) {
    bool remote = false;
    bool valid = false;
    typedef std::string::const_iterator iterator;
    const iterator& end(dest.end());
    for ( iterator i(dest.begin()); i != end; ++i) {
      if (*i == '@') remote = true;
      if (*i == ':') valid = true;
    }
    if (remote and valid) return true;
    return false;
  }

  // ToDo: Either it comes from another library, or it is a general facility
  // ToDo: Better name "split"
  // ToDo: Bad programming style.
  void strip(const std::string& fulldir, std::string& host, std::string& path) {
    assert(not fulldir.empty());
    char c = fulldir[0];
    unsigned int i = 0; 
    while (c != ':') {
      host += c;
      ++i;
      assert(i < fulldir.size());
      c = fulldir[i];
    }
    ++i;
    while (i < fulldir.size()) {
      c = fulldir[i];
      path += c;
      ++i;
    }
    assert(not path.empty());
    if (path[path.size() - 1] != '/') path += '/';
  }

  void verify_dest(const std::string& cmd_dest, container& backup_dir) {
    
    if (not is_remote(cmd_dest)) {
      fs::path dest_dir(fs::initial_path());
      dest_dir = fs::system_complete(fs::path(cmd_dest, fs::native));
      backup_dir.destination(dest_dir.native_file_string());
      if (not fs::exists(dest_dir)) throw Backup::Error::Directory_Error("Destination directory does not exist!");
      if (not fs::is_directory(dest_dir)) throw Backup::Error::Directory_Error("Destination specified is not a directory!");
    }
    else {
      backup_dir.destination(cmd_dest);
    }
  }

}
#endif
