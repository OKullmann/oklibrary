// Oliver Kullmann, 30.12.2008 (Swansea)
/* Copyright 2008, 2010, 2011 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Doxygen.hpp
  \brief Plans regarding building of Doxygen


  \bug Doxygen uses system C++ libraries
  <ul>
   <li> doxygen and doxytag use the system C++ libraries (libstdc++):
   \verbatim
bin> ldd doxygen 
        linux-vdso.so.1 =>  (0x00007fff719f4000)
        libpthread.so.0 => /lib/libpthread.so.0 (0x00007f466502c000)
        libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007f4664d26000)
        libm.so.6 => /lib/libm.so.6 (0x00007f4664aa2000)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007f466488c000)
        libc.so.6 => /lib/libc.so.6 (0x00007f4664509000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f466526c000)
ldd doxytag 
        linux-vdso.so.1 =>  (0x00007fff663ff000)
        libstdc++.so.6 => /usr/lib/libstdc++.so.6 (0x00007fc8f3d71000)
        libm.so.6 => /lib/libm.so.6 (0x00007fc8f3aee000)
        libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007fc8f38d7000)
        libc.so.6 => /lib/libc.so.6 (0x00007fc8f3554000)
        /lib64/ld-linux-x86-64.so.2 (0x00007fc8f409a000)
   \endverbatim
   whereas it should use the OKlibrary GCC libraries. </li>
   <li> We need to pass the LDFLAGS="$(gcc_linking_okl)" to
   Doxygen somehow, however, changing the configure line in
   the Makefile to:
   \verbatim
         LDFLAGS="$(gcc_linking_okl)" sh ./configure --prefix $(doxygen_install_directory_okl); $(postcondition) \
   \endverbatim
   has no effect.
   </li>
  </ul>


  \todo Install version 1.7.2

*/

