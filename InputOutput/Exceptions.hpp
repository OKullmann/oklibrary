// Oliver Kullmann, 14.10.2005 (Swansea)

/*!
  \file Exceptions.hpp
  \brief Exception classes (and related tools) for input and output of generalised satisfiability problems
  \todo Likely there should be an exception base class for the whole library.
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

    struct DimacsError : InputError {
      DimacsError(const std::string& m) : InputError(m) {}
    };

  }

}

#endif
