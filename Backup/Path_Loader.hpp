#ifndef PATH_LOADER_BACKUP_tjyahsaggi876qw93

#define PATH_LOADER_BACKUP_tjyahsaggi876qw93

#include <string>

#include <boost/filesystem/operations.hpp>

#include "Info_Holder.hpp"
#include "Backup.hpp"

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
    for (int i = 0; i <= dest.length(); ++i) {
      if (dest[i] == '@') remote = true;
      if (dest[i] == ':') valid = true;
    }
    if (remote and valid) return true;
    return false;
  }

  // ToDo: Either it comes from another library, or it is a general facility
  // ToDo: Better name "split"
  void strip(const std::string& fulldir, std::string& host, std::string& path) {
     char c = fulldir[0]; int i = 0; 
      while (c != ':') {
	host = host + c;
	++i;
	c = fulldir[i];
      }
      ++i;
      while (i < fulldir.length()) {
	c = fulldir[i];
	path = path + c;
	++i;
      }
      if (path[path.length() - 1] != '/') path = path + '/';
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
