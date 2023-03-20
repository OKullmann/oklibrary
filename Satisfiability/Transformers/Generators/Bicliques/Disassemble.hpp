// Oliver Kullmann, 19.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  General definitions regarding disassembly

*/

#ifndef DISASSEMBLE_Ao2I2cdXjQ
#define DISASSEMBLE_Ao2I2cdXjQ

#include <filesystem>

namespace Disassemble {

  std::filesystem::path extract_dir_path(const std::string& filename,
                                         const std::string& dirname) {
    if (dirname.empty())
      return std::filesystem::path(filename).stem();
    else
      return dirname;
  }

  enum class Error {
    missing_parameters = 1,
    input_file_error = 2,
    output_directory_error = 3
  };

}

#endif
