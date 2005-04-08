#ifndef BACKUP_asdfsdash82y9as

#define BACKUP_asdfsdash82y9as

#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

namespace Backup {
  
  std::string pfx(const std::string& s) {
    return "[Backup] " + s;
  }
  
  namespace DateTime {
    
    std::string current_date() {
      boost::gregorian::date d(boost::gregorian::day_clock::universal_day());
      return boost::gregorian::to_simple_string(d);
    }
    
     std::string current_time() {
      boost::posix_time::ptime t(boost::posix_time::second_clock::local_time());
      return boost::posix_time::to_simple_string(t.time_of_day());
    }
    
    std::string current_datetime() {
      boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());
      return boost::posix_time::to_simple_string(now);
    }
    
    
  }
  
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
