// Oliver Kullmann, 27.6.2007 (Swansea)

/*!
  \file Buildsystem/ExternalSources/SpecialBuilds/plans/Coq.hpp
  \brief Plans regarding installation of Coq
  
  
  \todo Install Coq (8.1)
  <ul>
   <li> Manual system-wide installation:
    <ol>
     <li> <code>tar -xzf coq-8.1.tar.gz</code> </li>
     <li> <code>cd coq-8.1</code> </li>
     <li> <code>./configure -opt --prefix /usr/local -fsets all -reals all</code> </li>
     <li> <code>make world</code> </li>
     <li> <code>sudo make install</code> </li>
    </ol>
    The manual says
    \verbatim
   If you wish to write tactics (and that really means that you belong
   to advanced users !) you *must* keep the Coq sources, without cleaning
   them.
    \endverbatim
    Now what does it %mean to "keep the sources" --- they recommend
    not to use "make install", but then the manual etc. won't
    be installed? We ignore this first, since writing tactics
    will only come later, and we can reinstall Coq anytime. </li>
   <li> Problems with "configure":
    <ol>
     <li> <code>LablGtk2 not found: CoqIde will not be available</code> This library comes normally with ocaml ?! </li>
    </ol>
   </li>
   <li> Problems with "make world":
    <ol>
     <li>
     \verbatim
> ./configure -opt --prefix /usr/local -fsets all -reals all
You have Objective-Caml 3.10.0. Good!
You have native-code compilation. Good!
LablGtk2 not found: CoqIde will not be available

  Coq top directory                 : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/coq-8.1
  Architecture                      : i686
  OS dependent libraries            : -cclib -lunix
  Objective-Caml/Camlp4 version     : 3.10.0
  Objective-Caml/Camlp4 binaries in : /usr/local/bin
  Objective-Caml library in         : /usr/local/lib/ocaml
  Camlp4 library in                 : +camlp4
  FSets theory                      : All
  Reals theory                      : All
  CoqIde                            : no

  Paths for true installation:
    binaries   will be copied in /usr/local/bin
    library    will be copied in /usr/local/lib/coq
    man pages  will be copied in /usr/local/man
    emacs mode will be copied in /usr/local/share/emacs/site-lisp

coq-8.1> make world
OCAMLC    config/coq_config.mli
OCAMLOPT  config/coq_config.ml
ECHO... > scripts/tolink.ml
OCAMLOPT  scripts/tolink.ml
OCAMLOPT  scripts/coqmktop.ml
OCAMLOPT -o bin/coqmktop.opt
cd bin; ln -sf coqmktop.opt coqmktop
OCAMLC    lib/pp_control.mli
OCAMLOPT  lib/pp_control.ml
OCAMLC    lib/pp.mli
OCAMLOPT4 lib/pp.ml4
OCAMLOPT  lib/compat.ml4
Camlp4: Uncaught exception: DynLoader.Error ("pa_ifdef.cmo", "file not found in path")

Preprocessor error
make: *** [lib/compat.cmx] Fehler 2
     \endverbatim
     ???
     </li>
     <li> Try to use the local ocaml-installation:
     \verbatim
tar -xzf coq-8.1.tar.gz
cd coq-8.1
export PATH=~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.10.0/bin:${PATH}
./configure -opt --prefix ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1 -fsets all -reals all
make world
make install
     \endverbatim
     Configure results in
     \verbatim
coq-8.1> ./configure -opt --prefix ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1 -fsets all -reals all
You have Objective-Caml 3.10.0. Good!
You have native-code compilation. Good!
LablGtk2 not found: CoqIde will not be available

  Coq top directory                 : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/coq-8.1
  Architecture                      : i686
  OS dependent libraries            : -cclib -lunix
  Objective-Caml/Camlp4 version     : 3.10.0
  Objective-Caml/Camlp4 binaries in : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.10.0/bin
  Objective-Caml library in         : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.10.0/lib/ocaml
  Camlp4 library in                 : +camlp4
  FSets theory                      : All
  Reals theory                      : All
  CoqIde                            : no

  Paths for true installation:
    binaries   will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/bin
    library    will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/lib/coq
    man pages  will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/man
    emacs mode will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/share/emacs/site-lisp

If anything in the above is wrong, please restart './configure'
     \endverbatim
     But then "make world" again yields
     \verbatim
coq-8.1> make world
OCAMLC    config/coq_config.mli
OCAMLOPT  config/coq_config.ml
ECHO... > scripts/tolink.ml
OCAMLOPT  scripts/tolink.ml
OCAMLOPT  scripts/coqmktop.ml
OCAMLOPT -o bin/coqmktop.opt
cd bin; ln -sf coqmktop.opt coqmktop
OCAMLC    lib/pp_control.mli
OCAMLOPT  lib/pp_control.ml
OCAMLC    lib/pp.mli
OCAMLOPT4 lib/pp.ml4
OCAMLOPT  lib/compat.ml4
Camlp4: Uncaught exception: DynLoader.Error ("pa_ifdef.cmo", "file not found in path")

Preprocessor error
make: *** [lib/compat.cmx] Fehler 2
     \endverbatim
     So it doesn't seem to be an ocaml-installation problem. In "Ocaml/3.10.0/lib/ocaml"
     there is no "pa_ifdef.cmo".
     </li>
     <li> Try to use the local ocaml-3.09.3-installation:
     \verbatim
tar -xzf coq-8.1.tar.gz
cd coq-8.1
export PATH=~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.09.3/bin:${PATH}
./configure -opt --prefix ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1 -fsets all -reals all
make world
make install
     \endverbatim
     Configure results in
     \verbatim
You have Objective-Caml 3.09.3. Good!
You have native-code compilation. Good!
LablGtk2 not found: CoqIde will not be available

  Coq top directory                 : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/coq-8.1
  Architecture                      : i686
  OS dependent libraries            : -cclib -lunix
  Objective-Caml/Camlp4 version     : 3.09.3
  Objective-Caml/Camlp4 binaries in : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.09.3/bin
  Objective-Caml library in         : /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.09.3/lib/ocaml
  Camlp4 library in                 : +camlp4
  FSets theory                      : All
  Reals theory                      : All
  CoqIde                            : no

  Paths for true installation:
    binaries   will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/bin
    library    will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/lib/coq
    man pages  will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/man
    emacs mode will be copied in /home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Coq/8.1/share/emacs/site-lisp
     \endverbatim
     Then
     \verbatim
make world
make install
cd doc
make all-ps
make all-pdf
     \endverbatim
     For ps and pdf the location of coqdoc.sty had to be entered by hand?
     html, refman, tutorial, faq doesn't work because of missing "hevea" ???
     "make stdlib" yields an error, and target "install" is not known???
     So nothing of
     \verbatim
make all-html
make refman
make tutorial
make stdlib
make faq
make DOCDIR=/home/kullmann/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/doc/Coq/8.1 install
     \endverbatim
     works. But manually
     \verbatim
doc> cp */*.ps ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/doc/Coq/8.1/
doc> cp */*.pdf ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/doc/Coq/8.1/
doc> cp */*.dvi ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/doc/Coq/8.1/
     \endverbatim
     works. Finally
     \verbatim
cd ..
make clean
     \endverbatim
     </li>
     <li> Perhaps the documentation really must be downloaded. </li>
    </ol>
   </li>
  </ul>


  \todo Improve ocaml installation
  <ul>
   <li> Where do we get lablgtk2 (the problems seems to be version 2) ??
   (The Coq installation claims it's not there, but it should
   have been build by the Ocaml installation.) </li>
   <li> Target "make cleanocaml" for removing the build-directory. </li>
   <li> What about (after "configure")
   \verbatim
NDBM not found, the "dbm" library will not be supported.
   \endverbatim
   </li>
  </ul>


  \todo Install ocaml (3.10.0) : DONE
  <ul>
   <li> ocaml and lablgtk are available with the Suse distribution --- so perhaps we simply don't bother about them? </li>
   <li> Manual installation (system-wide):
    <ol>
     <li> <code>tar -xjf ocaml-3.10.0.tar.bz2</code> </li>
     <li> <code>cd ocaml-3.10.0</code> </li>
     <li> <code>./configure</code> </li>
     <li> <code>make world</code> </li>
     <li> <code>make bootstrap</code> </li>
     <li> <code>make opt</code> </li>
     <li> <code>make opt.opt</code> </li>
     <li> <code>sudo make install</code> </li>
     <li> <code>make clean</code> </li>
    </ol>
   </li>
   <li> Target "make cleanocaml" then can remove the build-directory. </li>
   <li> Libraries "tk-devel, tcl-devel" need to be installed (Suse). </li>
   <li> What about (after "configure")
   \verbatim
NDBM not found, the "dbm" library will not be supported.
   \endverbatim
   ???
   </li>
   <li> Where do we get lablgtk2 (the problems seems to be version 2) ??
   (See below --- the Coq installation claims it's not there, but it should
   have been build by the Ocaml installation.) </li>
   <li> Since we need it only to install Coq, can we perform just
   a local installation?
    <ol>
     <li> Apparently all what is needed is that the bin-directory
     is on PATH? </li>
     <li> But likely ocaml is not just needed for the installation
     of Coq, but also later, and then it might create trouble. </li>
     <li> So we should just stick to the system-wide installation. </li>
     <li> But trying it out --- perhaps it solves the Coq-installation-problem ?
      <ol>
       <li>
       \verbatim
tar -xjf ocaml-3.10.0.tar.bz2; mkdir -p Ocaml/3.10.0; cd ocaml-3.10.0; \
./configure -prefix ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.10.0; \
make world && make bootstrap && make opt && make opt.opt && sudo make install
       \endverbatim
       </li>
      </ol>
     </li>
    </ol>
   </li>
  </ul>


  \todo Install ocaml 3.09.3 : DONE
  <ul>
   <li> Local installation
   \verbatim
tar -xzf ocaml-3.09.3.tar.gz; mkdir -p Ocaml/3.09.3; cd ocaml-3.09.3; \
./configure -prefix ~/csoliver/SAT-Algorithmen/OKplatform/ExternalSources/Coq/Ocaml/3.09.3; \
make world && make bootstrap && make opt && make opt.opt && sudo make install
   \endverbatim
   </li>
  </ul>

*/

