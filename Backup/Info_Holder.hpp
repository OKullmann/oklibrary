#ifndef INFO_HOLDER_BACKUP_JH865uj9l

#define INFO_HOLDER_BACKUP_JH865uj9l

#include <string>
#include <vector>

namespace InfoHolder {
  
  template <typename DataType>
  class DirectoryContainer {
    
    DataType source_dir;
    DataType destination_dir;
    DataType archive_name;
    
  public:
    
    void source(const DataType dir) {
      source_dir = dir;
    }
    
    DataType source() const {
      return source_dir;
    }
    
    void destination(const DataType dir) {
      destination_dir = dir;
    }
    
    DataType destination() const {
      return destination_dir;
    }

    void archive(const DataType name) {
      archive_name = name;
    }
    
    DataType archive() const {
      return archive_name;
    }
  
  };

}

#endif

