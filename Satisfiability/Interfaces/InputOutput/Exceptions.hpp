// Oliver Kullmann, 14.10.2005 (Swansea)
/* Copyright 2005 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Interfaces/InputOutput/Exceptions.hpp
  \brief Exception classes (and related tools) for input and output of generalised satisfiability problems


  \todo Likely there should be an exception base class for the whole library.
  \todo Perhaps like Module/messages there should be Module/exceptions ?
   Or should every module just contain a file Exceptions.hpp ?


  \todo A general standard for the messages of the exceptions is needed.
   - The Messages module should be used.
   - There should be a "full verbosity" level where amongst others the full description of the scope of the throwing location (like "OKlib::InputOutput::ClassX::functionf(int, Y&)") is given, while for "normal users" only the (simplified) text is shown.

*/

#ifndef EXCEPTIONS_610mnX
#define EXCEPTIONS_610mnX

#include <stdexcept>
#include <string>

namespace OKlib {
  namespace InputOutput {

    struct InputOutputError : std::runtime_error {
      InputOutputError(const std::string& m) : runtime_error(m) {}
    };

    struct InputError : InputOutputError {
      InputError(const std::string& m) : InputOutputError(m) {}
    };
    struct OutputError : InputOutputError {
      OutputError(const std::string& m) : InputOutputError(m) {}
    };

    struct IStreamError : InputError {
      IStreamError(const std::string& m) : InputError(m) {}
    };
    struct OStreamError : OutputError {
      OStreamError(const std::string& m) : OutputError(m) {}
    };

    struct DimacsInputError : InputError {
      DimacsInputError(const std::string& m) : InputError(m) {}
    };
    struct DimacsOutputError : OutputError {
      DimacsOutputError(const std::string& m) : OutputError(m) {}
    };

    struct CommentInputError : DimacsInputError {
      CommentInputError(const std::string& m) : DimacsInputError(m) {}
    };
    struct ParameterInputError : DimacsInputError {
      ParameterInputError(const std::string& m) : DimacsInputError(m) {}
    };
    struct ClauseInputError : DimacsInputError {
      ClauseInputError(const std::string& m) : DimacsInputError(m) {}
    };

    struct ParameterOutputError : DimacsOutputError {
      ParameterOutputError(const std::string& m) : DimacsOutputError(m) {}
    };
    struct ClauseOutputError : DimacsOutputError {
      ClauseOutputError(const std::string& m) : DimacsOutputError(m) {}
    };

  }

}

#endif
