// Oliver Kullmann, 19.2.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Programming/Messages/Utilities/Umlaute.hpp
  \brief Macros defining the UTF-8 code sequences for the seven special German
  characters.

  Instead of using these macros, it's easier to rely on the UTF-8 encoding of the
  source code files, and use the special characters directly (copying them from
  below, if necessary).

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
