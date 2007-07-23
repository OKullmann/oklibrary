// Oliver Kullmann, 19.7.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Sage.hpp
  \brief Plans regarding installation of Sage


  \todo Build Sage 2.6
  <ul>
   <li> Manual build
    <ol>
     <li> Download source from:
     \verbatim
http://modular.math.washington.edu/sage/dist/src/sage-2.6.tar
     \endverbatim
     </li>
     <li> In ExternalSources/Sage extract:
     \verbatim
tar xvf sage-2.6.tar
     \endverbatim
     </li>
     <li> Change directory:
     \verbatim
cd sage-2.6
     \endverbatim
     </li>
     <li> Build and install locally:
     \verbatim
make
     \endverbatim
     </li>
     <li> Build documentation:
     \verbatim
cd devel/doc
make pdf
     \endverbatim
     </li>
     <li>
     %Test:
     \verbatim
cd ../..
make test
     \endverbatim
     </li>
    </ol>
   </li>
   <li> We should link at least to the latex-documentation. It speaks about
   it in the README-file, however one can find only index.html (at several
   places!)? Apparently the README-file is not up-to-date, and meanwhile the
   html-documentation became the default. With the above "make pdf" however
   the latex-documentation is generated, and we should also link to it. DONE </li>
   <li> Create a special documentation page for Sage (the internal-link of the local
   home page links then to this special page), which contains all links to html-index
   pages and pdf-files one can find in the Sage-directory. DONE </li>
   <li> No indication of the documentation about "Axioms" etc. ?? DONE (not in the package) </li>
  </ul>


  \bug Building Sage 2.6 on csltmh
  \verbatim
make[1]: Leaving directory `/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6'
make[1]: Entering directory `/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6'
cd paper-letter && TEXINPUTS=/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/commontex: python /home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/tools/mkhowto --paper=letter --pdf ../ref/ref.tex
+++ TEXINPUTS=/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/ref:/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/commontex:/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/paper-letter:/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/texinputs:
+++ pdflatex ref
+++ makeindex ref.idx
+++ pdflatex ref
+++ makeindex -s /home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/texinputs/python.ist ref.idx
*** buffer overflow detected ***: makeindex terminated
======= Backtrace: =========
/lib/libc.so.6(__chk_fail+0x41)[0x4011b581]
/lib/libc.so.6(__strcpy_chk+0x43)[0x4011aa53]
makeindex[0x804ae8d]
makeindex[0x804b0fa]
/lib/libc.so.6(__libc_start_main+0xdc)[0x4005ff9c]
makeindex[0x8049481]
======= Memory map: ========
08048000-0805e000 r-xp 00000000 03:02 1429826    /usr/share/texmf/teTeX/bin/i586-linux/makeindex
0805e000-0805f000 r--p 00015000 03:02 1429826    /usr/share/texmf/teTeX/bin/i586-linux/makeindex
0805f000-08069000 rw-p 00016000 03:02 1429826    /usr/share/texmf/teTeX/bin/i586-linux/makeindex
08069000-08112000 rw-p 08069000 00:00 0          [heap]
40000000-4001b000 r-xp 00000000 03:02 4063244    /lib/ld-2.5.so
4001b000-4001d000 rw-p 0001a000 03:02 4063244    /lib/ld-2.5.so
4001d000-4001e000 r-xp 4001d000 00:00 0          [vdso]
4001e000-4001f000 rw-p 4001e000 00:00 0
4004a000-40172000 r-xp 00000000 03:02 4063251    /lib/libc-2.5.so
40172000-40173000 r--p 00128000 03:02 4063251    /lib/libc-2.5.so
40173000-40175000 rw-p 00129000 03:02 4063251    /lib/libc-2.5.so
40175000-40179000 rw-p 40175000 00:00 0
40179000-40183000 r-xp 00000000 03:02 8962340    /usr/local/lib/libgcc_s.so.1
40183000-40184000 r--p 00009000 03:02 8962340    /usr/local/lib/libgcc_s.so.1
40184000-40185000 rw-p 0000a000 03:02 8962340    /usr/local/lib/libgcc_s.so.1
bfdac000-bfdc9000 rw-p bfdac000 00:00 0          [stack]
sh: line 1:   557 Aborted                 ( makeindex -s /home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/texinputs/python.ist ref.idx ) </dev/null >>/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/paper-letter/ref.how 2>&1
*** Session transcript and error messages are in /home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/paper-letter/ref.how.
*** Exited with status 134.
The relevant lines from the transcript are:
------------------------------------------------------------------------
+++ makeindex -s /home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/texinputs/python.ist ref.idx
*** Session transcript and error messages are in /home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6/paper-letter/ref.how.
*** Exited with status 134.
make[1]: *** [paper-letter/ref.pdf] Error 134
make[1]: Leaving directory `/home/csmatthew/OKplatform/ExternalSources/Sage/sage-2.6/devel/doc-2.6'
make: *** [sage-2.6] Error 1
  \endverbatim


  \todo Building Sage 2.7
  <ul>
   <li> Error related to gfortran:
   \verbatim
Error installing Fortran: On your system, you must install the gfortran VERSION 4.x compiler *manually* before installing SAGE.
   \endverbatim
   however gfortran 4.0.2 is installed (/usr/bin/gfortran), and apparently
   the Sage build process realised this?
   </li>
   <li> Perhaps we could offer to install gfortran belonging to gcc-4.1.2
   systemwide? </li>
  </ul>

*/
