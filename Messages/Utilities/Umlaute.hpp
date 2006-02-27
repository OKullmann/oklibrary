// Oliver Kullmann, 19.2.2006 (Swansea)

/*!
  \file Messages/Utilities/Umlaute.hpp
  \brief Macros defining the UTF-8 code sequences for the seven special German
  characters.

  Instead of using these macros, it's easier to rely on the UTF-8 encoding of the
  source code files, and use the special characters directly (copying them from
  below, if necessary).
  \todo Is it guaranteed that the UTF-8 encoding of the file will translate into
  the same encoding of the string literals?
*/

#ifndef UMLAUTE_yryry55rer

#define UMLAUTE_yryry55rer

namespace OKlib {
  namespace Messages {
    namespace Utilities {

#define OKLIB_aE "\xC3\xA4" // ä
#define OKLIB_AE "\xC3\x84" // Ä
#define OKLIB_oE "\xC3\xB6" // ö
#define OKLIB_OE "\xC3\x96" // Ö
#define OKLIB_uE "\xC3\xBC" // ü
#define OKLIB_UE "\xC3\x9C" // Ü
#define OKLIB_SS "\xC3\x9F" // ß

    }
  }
}

#endif
