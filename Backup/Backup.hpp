#ifndef BACKUP_asdfsdash82y9as

#define BACKUP_asdfsdash82y9as

#include <string>

namespace Backup {
    
  namespace Error {
    struct Directory_Error {
      Directory_Error(const std::string& message) : message(message) {} 
      std::string what() const {
	return "Directory Error: " + message;
      }
    private :
      std::string message;
    };
    
    struct Command_Error {
      Command_Error(const std::string& message) : message(message) {} 
      std::string what() const {
	return "Command Error: " + message;
      }
    private :
      std::string message;
    };
  }
}

#endif
