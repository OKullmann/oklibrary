// Oliver Kullmann, 30.10.2005 (Swansea)

/*!
  \file OptionBase.hpp
  \brief Base class for options.
  \todo Program options shall be handled as before "Kommandozeile" (that is, not
  "options", but commands, to be processed from left to right), but now object-oriented.
  Program options represent command-complexes (parsing the command, executing
  it, handling error situations, and output of help- and info-information), which are
  put into some container used to interprete the sequence of strings comming from
  the command line (possible execution modes: trying all out and error if not exactly
  one option is applicable, or just considering the first option found).
  There are always the following standard options:
   - --help
   - --info
   - --language=
   - --verbosity=
*/

#ifndef OPTIONBASE_nxnnzLLk

#define OPTIONBASE_nxnnzLLk

namespace OKlib {

  namespace ProgramOptions {

  }

}

#endif
