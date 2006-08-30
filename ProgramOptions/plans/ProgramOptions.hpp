// Oliver Kullmann, 30.10.2005 (Swansea)

/*!
  \file ProgramOptions/plans/ProgramOptions.hpp
  \brief Plans for the module on program options (as a whole).

  \todo The OLD point of view:

  Program options shall be handled as before
  "Kommandozeile" (that is, not "options", but commands, to be processed from left to right),
  but now object-oriented.
  Program options represent command-complexes (parsing the command, executing
  it, handling error situations, and output of help- and info-information), which are
  put into some container used to interprete the sequence of strings coming from
  the command line (possible execution modes: trying all out and error if not exactly
  one option is applicable, or just considering the first option found).
  There are always the following standard options:
   - --help
   - --info
   - --language=
   - --verbosity=

  \todo The NEW point of view: It doesn't seem good design to me (OK) anymore that
  some kind of "universal program execution scheme" is invented here; in other words,
  it seems better to me now that the program-options module only provides means
  for providing the program parameters and for processing the *general* options (regarding
  help and documentation), while the interpretation of the specific "options" (or "commands")
  is not covered at all.
  \todo Programs can have three different kind of parameters (as strings), called "program parameters"
  from now on:
   - Options, which always exists in the long form "--option" and in the short form
     "-opt".
   - Assignments, which again always exist in the long form "variable=..." and
     in the short form "var=...".
   - Free options, which is everything else (possibly also of the form "--string" for example,
     but not matching any of the given options or assignments).
  Perhaps as a general guideline the short forms of options and assignments should consist
  of 3 letters (using a single letter only in case of well-established quasi-standards).
  \todo The following standard options are enabled by the system:
   - "--help" ("-h") for help, listing all options and assignments with explanations (starting
     with the standard options and assignments).
   - "--version" ("-v") showing version information.
   - "--license" ("-lcs") showing licence information.
   - "--purpose" ("-prp") for some text explaining the overall purpose of the program.
   - "--usage" ("-usg") for some text explaining the general usage pattern.
   - "--overview" ("-ovw") as a shorthand for
     "--purpose --usage --help --version -- licence".
  And there are three general assignments:
   - "--language=..." ("-lng=... ")
   - "--detailedness-level=..." ("-dtl=...")
   - "--verbosity-level=..." ("-vbl=...").
  The first two assignments effect the message system (and thus also the above six general
  options), while with verbosity-level we can ask for explanations. Perhaps verbosity-level
  is just a natural number (with 0 for no explanations), while detailedness-level is (as
  outlined in Messages/Messages_Plans.hpp) one of basic, full or extensive.
  Program parameters different from these standard options and assignments are called
  "special program parameters" (while the above standard options and assignments
  are "standard program parameters").
  \todo There is an enumeration StandardProgramParameters (typedef StdPP for short)
  enum StandardProgramParameters { other_std = 0, help, version, license, purpose,
    usage, overview, language, verbosity_level };
  \todo For a new "program option system" a class has to be defined; lets calls it POS here.
  POS contains an enumeration SpecialProgramParameters (typedef SpecPP for short) with
  value other_spec = 0, and for every other special program parameter exactly one value.
  Then there is the (nested) structure
  struct ProgramParameter {
    // type member string_type
    StandardProgramParameters std_p;
    SpecialProgramParameters spec_p;
    string_type val;
  };
  val shall contain the assignment value for assignments, and the whole parameter string
  for free options, while otherwise val shall be the empty string. At least one of std_p
  and spec_p shall be zero.
  A vector data member of POS contains for every index associated with a value of SpecPP
  a pair consisting of a message object (for "--help") and a Specification object (see below).
  There are message objects for version, licence, purpose
  and usage (while the general system provides predefined message objects for help, overview,
  language and verbosity-level).
  And the general system provides a default message object in case the program is called without
  parameters, but parameters are needed.
  \todo The short form of "--version" contains program name and version number,
  while the long form has additionally author names, compilation date, OKlib-version-number,
  filename, line number.
  \todo The general system provides a class
  struct Specification {
    bool is_assignment;
    std::string short;
    std::string long;
  };
  \todo An object P of class type AnalyseProgramOptions is constructed with two streams out and err
  (corresponding to cout and cerr), and gives access to the message system (so that the POS object can
  supply messages reacting appropriately to the existing languages and verbosity-levels).
  P also has variables current_language and current_verbosity_level.
  Then a POS object is given to P, and potentially analysed in order to enable fast analysis
  of program options (using some string algorithm). Since POS cannot be completely
  known to P (since POS depends on AnalyseProgramOptions), likely POS should be
  part of a polymorphic hierarchy.
  Finally a range of strings (the program parameters) is given to P, which enables
  then access to an InputRange of objects of type ProgramParameter, corresponding
  to the program parameters.
  In case of standard program parameters output is made to out, while in case no free
  options are allowed (the default) in case of an unrecognised option an error message
  is made to err (but continues otherwise; for the user this case is recognisable by the fact
  that the two enumeration values are both zero).
  The program options are evaluated one by one (so that there
  is no overhead in case of a long list of program options); only when the iterator advances
  a new program option is considered.
  \todo Every program should just pass the three standard streams std::cout, std::cerr and
  std::cin and potentially count and argv to the constructor of some "program object",
  which runs then the actual actions. In this way we have a chance to test whole programs
  within our ("unit") test system. There should be a concept for such program objects,
  but likely no class hierarchy is useful here (one just has four forms of a constructor:
  without any parameters, with the three standard streams, with the program parameters,
  and with all five parameters).
  \todo The boost program options library has the following weaknesses:
   - no reusable components are delivered (but only a "blob");
   - not generic;
   - everything shall in principle be arranged by one function call, which then has to
     care for (too) many things.
   This (failed) library teaches us, that "convenience" should only by a "facade", and
   should be based on a solid technical base made out of reusable components.
   \todo The old system Kommandozeile needs to be inspected for some "goodies".
*/

/*!
  \namespace OKlib::ProgramOptions
  \brief Components for handling command-line parameters
*/

namespace OKlib {
  namespace ProgramOptions {
  }
}

