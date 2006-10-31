# Oliver Kullmann, 6.3.2002 (Swansea)

# ===============================================================================
# Enviroment/Make build system variables
# ===============================================================================

# TEMPORARY COMMENT OK: All the variables like CXX and CXXFLAGS are missing (but they
# are important).

#
# The most important build system parameters come as environment/Make variable
# pairs. 
# 
# (OKPLATFORM,OKplatform)
# (OKSYSTEM,OKsystem)
# (SYSTEMDIRECTORIES,SystemDirectories)
# (BOOST,Boost)
#
# In each case the default behaviour is for the build system to use the 
# environment variable unless the Make variable is defined, in which case
# the Make variable is used.
#
# OKPLATFORM/OKplatform
#
# This variable pair specifies the top level directory of the OKplatform.
# The subdirectories "ExternalSources" and $(SystemDirectories) are created here by 
# the build system.

# TEMPORARY COMMENT OK: We should make a difference between a variable-name and
# a value. Shouldn't we have also a variable for "ExternalSources" ?

#
# If both the environment variable OKPLATFORM and the Make variable OKplatform 
# are undefined then the build system gives an error message.
#
# OKSYSTEM/OKsystem
#
# This variable pair specifies the top level directory of the OKsystem
# source tree. The build system assumes that the directory specified by the value
# of this variable is the directory containing the subdiretories "Annotations",
# "Transitional" and "OKlibrary".
#
# SYSTEMDIRECTORIES/SystemDirectories
#
# This variable pair specifies the location of the top level directory of 
# the OKlibrary build tree. The build system creates the subdirectories "aux", "bin", 
# "doc" and "lib" in the directory specified by the value of this variable.
#
# BOOST/Boost
#
# This variable pair specifies the location of the installation directory 
# of the Boost distribution to be used by the build system for compiling the 
# library.
#
# ===============================================================================
# Targets
# ===============================================================================
#
# Applications
# ------------
# all                           Compile unoptimised and optimised object files 
#				and programs.
# unoptimised                   Compile unoptimised object files and programs.
# optimised                     Compile optimised object files and programs.
#
# Documentation
# -------------
# html                          Create doxygen documentation.
#
# Test system
# -----------
# check                         Perform unoptimised and optimised tests from old
#                               test system.
# new_check                     Perform unoptimised and optimised tests from new
#                               test system.
# test                          Perform unoptimised tests from old test system.
# testop                        Perform optimised tests from old test system.
# new_test                      Perform unoptimised tests from new test system.
# new_testop                    Perform optimised tests from new test system.
#
# Cleaning
# --------
# cleanall                      Remove object files, test object files, 
#                               dependency files, test timestamps, applications 
#                               and test programs
# clean                         Remove object files, test object files, 
#                               dependency files and test timestamps.
# cleanprograms                 Remove all applications and test programs.
# cleanalltests                 Remove timestamp files for unoptimsed and 
#                               optimised test program from old test systems.
# new_cleanalltests             Remove timestamp file for unoptimised and 
#                               optimised test program from new test system.
# cleantest                     Remove timestamp file for unoptimised test 
#                               program from old test system.	
# cleantestop                   Remove timestamp file for optimised test program 
#                               from old test system.
# new_cleantest                 Remove timestamp file for unoptimised test 
#				program from new test system.
# new_cleantestop               Remove timestamp file for optimised test program 
#                               from new test system.
# clean_obj                     Remove all non-test object files.
# cleantestobj                  Remove all test object files.
# cleandep                      Remove all dependency files.
# cleanprograms                 Remove all applications and test programs.
#
# ===============================================================================
# User variables: 
# ===============================================================================
#
# Variables for doxygen
# ---------------------
# doxygen-parameters    For doxygen parameters.
#

# #############################################################

SHELL = /bin/sh
.SUFFIXES :

define last-element
$(word $(words $1),$1)
endef

ifndef srcdir

this-makefile := $(call last-element,$(MAKEFILE_LIST))
other-makefiles := $(filter-out $(this-makefile),$(MAKEFILE_LIST))
parent-makefile := $(call last-element,$(other-makefiles))
srcdir := $(shell cd $(dir $(parent-makefile)); pwd)
module-name := $(notdir $(srcdir))

endif

ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile!)
  endif
endif

ifndef OKsystem
  ifdef OKSYSTEM
    OKsystem := $(OKSYSTEM)
  else
    OKsystem := $(OKplatform)/OKsystem
  endif
endif

OKsystem_include := -I$(OKsystem)

OKBuildsystem := $(OKsystem)/Transitional/Buildsystem

ifndef Boost
  ifdef BOOST
    Boost = -I$(BOOST)
  endif
endif

export

include $(OKBuildsystem)/makefile_standardgoals.mak

export

include $(srcdir)/makefile.definitions.mak

# Definitions required from makefile.definitions:
# General_options
# Optimisation_options
# test_program (for the old test-system)
# programs
# source_libraries
# link_libraries

source_libraries += $(OKsystem_include)

prefix := $(OKplatform)

ifndef SystemDirectories
  ifdef SYSTEMDIRECTORIES
    SystemDirectories := $(SYSTEMDIRECTORIES)
  else
    SystemDirectories := $(prefix)/system_directories
  endif
endif

system_directories := $(SystemDirectories)

bindir := $(system_directories)/bin
libdir := $(system_directories)/lib

aux_dir := $(system_directories)/aux
latex_dir := $(aux_dir)/latex
dependencies_dir := $(aux_dir)/dependencies

doc_dir := $(system_directories)/doc
html_dir := $(doc_dir)/html

test-bindir := $(bindir)/tests
test-libdir := $(libdir)/tests/$(module-name)
test-auxdir := $(aux_dir)/tests/$(module-name)

directories := $(bindir) $(libdir) $(aux_dir) $(latex_dir) $(dependencies_dir) $(doc_dir) $(html_dir) $(test-bindir) $(test-libdir) $(test-auxdir)

doxygen-parameters := 
Doxygen_modifier := 2> $(aux_dir)/DoxygenErrorMessages

# -----------------------------------------------------------------------------------

ifneq ($(programs),)
  programs := $(addprefix $(bindir)/, $(programs))
endif

ifneq ($(test_program),) # old test-system needed
  test_program := $(addprefix $(bindir)/, $(test_program))
  programs += $(test_program)
endif

new_test_program := $(test-bindir)/$(module-name)
standard_test_program_object_file := $(libdir)/TestProgram_DesignStudy.o

testobjects-dir := $(srcdir)/testobjects

# ---------------------------------------------------

name_addition := $(shell echo $(Optimisation_options) | tr --delete " ")
All_options := $(General_options) $(Optimisation_options)

programs_optimised := $(programs:=$(name_addition))

new_test_program_optimised := $(new_test_program)$(name_addition)
standard_test_program_object_file_optimised := $(libdir)/TestProgram_DesignStudy$(name_addition).o

compilation_units_cpp := $(wildcard $(srcdir)/*.cpp)
compilation_units_cpp := $(notdir $(compilation_units_cpp))
compilation_units_c := $(wildcard $(srcdir)/*.c)
compilation_units_c := $(notdir $(compilation_units_c))

test_compilation_units := $(wildcard $(testobjects-dir)/*.cpp)

dependency_files_cpp := $(compilation_units_cpp:.cpp=.d)
dependency_files_cpp := $(addprefix $(dependencies_dir)/, $(dependency_files_cpp))
dependency_files_c := $(compilation_units_c:.c=.d) 
dependency_files_c := $(addprefix $(dependencies_dir)/, $(dependency_files_c))
dependency_files := $(dependency_files_cpp) $(dependency_files_c)

test_dependency_files := $(notdir $(test_compilation_units))
test_dependency_files := $(test_dependency_files:.cpp=.d)
test_dependency_files := $(addprefix $(test-auxdir)/, $(test_dependency_files))
dependency_files := $(dependency_files) $(test_dependency_files)

object_files_cpp := $(compilation_units_cpp:.cpp=.o) 
object_files_cpp := $(addprefix $(libdir)/, $(object_files_cpp))
object_files_c := $(compilation_units_c:.c=.o)
object_files_c := $(addprefix $(libdir)/, $(object_files_c))
object_files := $(object_files_cpp) $(object_files_c)
object_files_cpp_optimised := $(compilation_units_cpp:.cpp=$(name_addition).o)
object_files_cpp_optimised := $(addprefix $(libdir)/, $(object_files_cpp_optimised))
object_files_c_optimised := $(compilation_units_c:.c=$(name_addition).o)
object_files_c_optimised := $(addprefix $(libdir)/, $(object_files_c_optimised))
object_files_optimised := $(object_files_cpp_optimised) $(object_files_c_optimised)

test_object_files := $(notdir $(test_compilation_units))
test_object_files := $(test_object_files:.cpp=.o)
test_object_files := $(addprefix $(test-libdir)/, $(test_object_files))

test_object_files_optimised := $(notdir $(test_compilation_units))
test_object_files_optimised := $(test_object_files_optimised:.cpp=$(name_addition).o)
test_object_files_optimised := $(addprefix $(test-libdir)/, $(test_object_files_optimised))


test_file := $(srcdir)/test
testop_file := $(srcdir)/testop

# -----------------------------------------------------------------------------------

doxy_file := $(OKBuildsystem)/Doxyfile

# -----------------------------------------------------------------------------------

CXX := g++
CC := gcc
Standard_options := -ansi -pedantic
Warning_options := -Wall
Compile_tool := time -p
Link_tool := time -p
Test_tool := time -p

# -----------------------------------------------------------------------------------

test_level := basic

error_file := $(test-auxdir)/Error
message_file := $(test-auxdir)/Message
log_file := $(test-auxdir)/Log

error_stream := "cerr|ofstream-w=$(error_file)|ofstream-w=$(log_file)"
message_stream := "cout|ofstream-w=$(message_file)|ofstream-w=$(log_file)"
log_stream := "ofstream-w=$(log_file)"

error_level := 0
message_level := 0
log_level := 0

error_lang := en_GB
message_lang := en_GB
log_lang := en_GB

test_parameters := $(test_level) $(error_stream) $(message_stream) $(log_stream) $(error_level) $(message_level) $(log_level) $(error_lang) $(message_lang) $(log_lang)

# -----------------------------------------------------------------------------------

test_timestamp := $(test-auxdir)/new_test_$(test_level)
testop_timestamp := $(test-auxdir)/new_testop_$(test_level)

# -----------------------------------------------------------------------------------

include $(OKBuildsystem)/external_sources_versions.mak

alternative_gcc:= $(gcc_recommended_version_number)
alternative_boost:= $(boost_recommended_version_number)
alternative_library_path:=

ifneq ($(alternative_gcc),) # alternative gcc specified
  CXX := $(prefix)/ExternalSources/Gcc/$(alternative_gcc)/bin/g++
  CC := $(prefix)/ExternalSources/Gcc/$(alternative_gcc)/bin/gcc
  alternative_library_path:= -L$(prefix)/ExternalSources/Gcc/$(alternative_gcc)/lib64 -Wl,-rpath,$(prefix)/ExternalSources/Gcc/$(alternative_gcc)/lib64 -L$(prefix)/ExternalSources/Gcc/$(alternative_gcc)/lib -Wl,-rpath,$(prefix)/ExternalSources/Gcc/$(alternative_gcc)/lib
  ifeq ($(alternative_boost),)
    ifndef Boost
      $(error Variable Boost must be defined!)
    endif
    alternative_boost := $(patsubst -I%,%,$(Boost))
    alternative_boost := $(patsubst %/,%,$(alternative_boost))
    alternative_boost := $(notdir $(alternative_boost))
    alternative_boost := $(patsubst boost-%,%,$(alternative_boost))
  endif
  alternative_library_path := $(alternative_library_path) -L$(prefix)/ExternalSources/Boost/$(alternative_boost)+$(alternative_gcc)/lib -Wl,-rpath,$(prefix)/ExternalSources/Boost/$(alternative_boost)+$(alternative_gcc)/lib
  Boost = -I$(prefix)/ExternalSources/Boost/$(alternative_boost)+$(alternative_gcc)/include/boost-$(alternative_boost)
else # no alternative gcc specified
  ifneq ($(alternative_boost),) # alternative boost specified
  alternative_library_path := $(alternative_library_path) -L$(prefix)/ExternalSources/Boost/$(alternative_boost)/lib -Wl,-rpath,$(prefix)/ExternalSources/Boost/$(alternative_boost)/lib
  Boost = -I$(prefix)/ExternalSources/Boost/$(alternative_boost)/include/boost-$(alternative_boost)
  endif
endif

# Explanation: With "-Wl,-rpath,Path" we tell the compiler to pass the option "-rpath Path" to the linker, which will add Path to the places searched for this program for run-time-libraries.

# ----------------------------------------------------------------

all : unoptimised optimised

$(directories) :
	@mkdir -p $@

html : $(html_dir) 
	echo "Doxygen version: $$(doxygen --version)"; rm -r $(html_dir)/*; cd $(OKplatform); ( cat $(doxy_file); echo $(doxygen-parameters) "OUTPUT_DIRECTORY=$(doc_dir)" ) | doxygen - $(Doxygen_modifier)

unoptimised : $(object_files) $(programs)

optimised : $(object_files_optimised) $(programs_optimised)

check : test testop

new_check : new_test new_testop

ifneq ($(test_program),) # old test-system needed
test : $(test_file)
testop : $(testop_file)
else
test :
testop :
endif

new_test : $(test_timestamp)
new_testop : $(testop_timestamp)

$(test_file) : $(test_program)
	$(Test_tool) $(test_program)
	touch $(test_file)
$(testop_file) : $(test_program)$(name_addition)
	$(Test_tool) $(test_program)$(name_addition)
	touch $(testop_file)

$(test_timestamp) : $(new_test_program)
	$(call conditional_mkdir, $(test-auxdir))
	$(Test_tool) $(new_test_program) $(test_parameters)
	touch $(test_timestamp)
$(testop_timestamp) : $(new_test_program_optimised)
	$(Test_tool) $(new_test_program_optimised) $(test_parameters)
	touch $(testop_timestamp)

force :

# ---------------------------------------------------------------

ifeq ($(firstword $(filter $(special_goals) $(cleaning_goals) $(forced_goals), $(MAKECMDGOALS))),)
include $(dependency_files)
endif

$(dependency_files_cpp) : $(dependencies_dir)/%.d : $(srcdir)/%.cpp | $(dependencies_dir)
	$(CXX) -MM -MF $@ -MT $(libdir)/$*.o -MT $(libdir)/$*$(name_addition).o -MT $@ $(source_libraries) $<

$(dependency_files_c) : $(dependencies_dir)/%.d : $(srcdir)/%.c | $(dependencies_dir)
	$(CC) -MM -MF $@ -MT $(libdir)/$*.o -MT $(libdir)/$*$(name_addition).o -MT $@ $(source_libraries) $<

$(test_dependency_files) : $(test-auxdir)/%.d : $(testobjects-dir)/%.cpp | $(test-auxdir)
	$(CXX) -MM -MF $@ -MT $(test-libdir)/$*.o -MT $(test-libdir)/$*$(name_addition).o -MT $@ $(source_libraries) $<

$(object_files_cpp_optimised) : $(libdir)/%$(name_addition).o : $(srcdir)/%.cpp | $(libdir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(Optimisation_options) $(source_libraries) $<

$(object_files_cpp) : $(libdir)/%.o : $(srcdir)/%.cpp | $(libdir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(General_options) $(source_libraries) $<

$(object_files_c_optimised) : $(libdir)/%$(name_addition).o : $(srcdir)/%.c | $(libdir)
	$(Compile_tool) $(CC) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CFLAGS) $(Optimisation_options) $(source_libraries) $<

$(object_files_c) : $(libdir)/%.o : $(srcdir)/%.c | $(libdir)
	$(Compile_tool) $(CC) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CFLAGS) $(General_options) $(source_libraries) $<

$(test_object_files) : $(test-libdir)/%.o : $(testobjects-dir)/%.cpp | $(test-libdir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(General_options) $(source_libraries) $<

$(test_object_files_optimised) : $(test-libdir)/%$(name_addition).o : $(testobjects-dir)/%.cpp | $(test-libdir)
	$(Compile_tool) $(CXX) -c -o $@ $(Standard_options) $(Warning_options) $(CPPFLAGS) $(CXXFLAGS) $(Optimisation_options) $(source_libraries) $<


define get-link_libraries
$$(if [[ -e $(srcdir)/$*.link_libraries ]]; then $(srcdir)/$*.link_libraries; else echo $(link_libraries); fi)
endef
define get-link_libraries_optimised
$$(if [[ -e $(srcdir)/$*.link_libraries_optimised ]]; then $(srcdir)/$*.link_libraries_optimised; else if [[ -e $(srcdir)/$*.link_libraries ]]; then $(srcdir)/$*.link_libraries; else echo $(link_libraries); fi; fi)
endef

export

$(programs) : $(bindir)/% : $(libdir)/%.o | $(bindir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(General_options) $< $(alternative_library_path) $(get-link_libraries)

$(programs_optimised) : $(bindir)/%$(name_addition) : $(libdir)/%$(name_addition).o | $(bindir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(Optimisation_options) $< $(alternative_library_path) $(get-link_libraries_optimised)

$(new_test_program) : $(test-bindir)/% : $(test_object_files) $(standard_test_program_object_file) | $(test-bindir) $(test-auxdir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(General_options) $^ $(alternative_library_path) $(get-link_libraries)

$(new_test_program_optimised) : $(test-bindir)/%$(name_addition) : $(test_object_files_optimised) $(standard_test_program_object_file_optimised) | $(test-bindir) $(test-auxdir)
	$(Link_tool) $(CXX) -o $@ $(Standard_options) $(Warning_options) $(Optimisation_options) $^ $(alternative_library_path) $(get-link_libraries_optimised)

# --------------------------------

cleantest :
	- rm $(test_file)

cleantestop :
	- rm $(testop_file)

cleanalltest : cleantest cleantestop

new_cleantest :
	- rm $(test_timestamp)

new_cleantestop :
	- rm $(testop_timestamp)

new_cleanalltests : new_cleantest new_cleantestop

cleanobj :
	- rm $(object_files) $(object_files_optimised)

cleantestobj :
	- rm $(test_object_files) $(test_object_files_optimised)

cleandep :
	- rm $(dependency_files)

clean : cleanobj cleantestobj cleandep
	- rm $(test_file) $(testop_file) $(test_timestamp) $(testop_timestamp) 

cleanprograms :
	- rm $(programs) $(programs_optimised) $(new_test_program) $(new_test_program_optimised) 

cleanall : clean cleanprograms
	- rm $(error_file) $(message_file) $(log_file)

# TEMPORARY COMMENT OK
# It's quite unusual to put this at the end (most people will overlook that --- including me).
# I propose to put the system variables also at the top (perhaps with a separation).
# And the special variables CXX, ... belong to a place still further up.
# You have CXX, but not CXXFLAGS ... ?
# Regarding the placement, perhaps we should have sub-make-files for the different parts
# (compilation, documentation, etc.) ?! Then the special documentation could go to these
# sub-make-files.
# Please check, whether it's still true that we can call a generic make-file from other places.

# ===============================================================================
# System variables (can be optionally redefined) : 
# ===============================================================================

# TEMPORARY COMMENT OK
# One should say here, that any redefinition needs to be thoroughly thought through.

#
# bindir                Directory for placement of non-test executables
# libdir                Directory for placement of object files and link 
#                       libraries
# aux_dir               Directory for placement of doxygen error messages log. 
#                       Contains subdirectories dependencies, latex and tests.
# latex_dir             Directory for placement of latex aux files.
# dependencies_dir      Directory for placement of dependency files.
# doc_dir               Directory containing subdirectories dvi and html.
# html_dir              Directory for placement of doxygen documentation.
# test-bindir           Directory for placement of all test executables.
# test-libdir           Directory for placement of module test object files.
# test-auxdir           Directory for placement of module test Error, Log, 
#                       Message and test timestamp files.
#
# Variables for doxygen
# ---------------------
# Doxygen_modifier      Modifier to doxygen command, for example to redirect 
#                       output to a log file.
# doxy_file             Location of Doxygen config file.
#
# Variables for compiler & compiler options
# -----------------------------------------
# CXX                   Name of the C++ compiler executable.
# CC                    Name of the C compiler executable.
# General_options       Defined in makefile_definitions.mak for each module.
#                       Passed to the compiler for each module program.
# Optimisation_options  Defined in makefile_definitions.mak for each module.
#                       Passed to the compiler for each module program. Also
#                       used as the addition to the name of executables.
# source_libraries      Defined in makefile_definitions.mak for each module
#                       and extended by makefile_generic.mak. Passed to the
#                       compiler for each module program. Intended to contain
#                       a list of directories (each preceeded with -I) for the
#                       compiler to search for header files.
# link_libraries        Defined in makefile_definitions.mak for each module.
#                       Passed to the compiler for each module program.
#                       Intended to contain a list, where each element of the
#                       list has the form "-LX -lY", where X is a path to a
#                       directory for the compiler to search for link libraries
#                       and Y is the name of some link library.
# name_addition         A string to be added to the name of each program.
#
# Standard_options      Defined in makefile_generic.mak. Passed to the 
#                       compiler for every program.
# Warning_options       Defined in makefile_generic.mak. Passed to the 
#                       compiler for every program.

# TEMPORARY COMMENT OK: The *meaning* of the variables like "Standard_options" and so
# on is most important! You tell only some formalities. 
# Also when reflecting about the meaning, the basic step for
# a rational reorganisation is done!

# Compile_tool          A command to prefix each call of the compiler. So, for
#                       example to use the time program to time the execution
#                       of the compiler.
# Link_tool             A command to prefix each call of the linker. So, for
#                       example to use the time program to time the execution
#                       of the linker.
# Test_tool             A command to prefix each call of a test program. So, for
#                       example to use the time program to time the execution
#                       of the test program.

# ===============================================================================
# Internal variables: 
# ===============================================================================
#
# directories		All subdirectories of system directories.
#
# Variables for the directory structure of system directories.
# ------------------------------------------------------------
#
# Variables for directory structure of srcdir
# -------------------------------------------
# srcdir                The source code directory of the "current" module.
# testobjects-dir       The directory of test object files for the current 
#                       module.
#
# Variables for programs
# ----------------------
# programs                              Defined in makefile_definitions.mak
#                                       for each module. Specifies names of
#                                       applications for current module.
# test_program (deprecated)             The name of the current module test
#                                       program  (old test system).
# new_test_program                      The name of the current module test
#                                       program.
# standard_test_program_object_file     Specifies the location of the test
#                                       program object file.
#
# Variables for compiler & compiler options
# -----------------------------------------
#
# All_options           The concatenation of General_options and 
#                       Optimisation_options.
#
# Main compilation variables
# --------------------------
#
# compilation_units_cpp         The list of C++ source code files for the 
#                               applications in the current module.
# compilations_units_c          The list of C source code files for the
#                               applications in the current module.
# test_compilation_units        The list of C++ source code files for the
#                               tests in the current module.
# dependency_files_cpp          The list of dependency files for the C++
#                               source code files in the current module.
# dependency_files_c            The list of dependency files for the C
#                               source code files in the current module.
# dependency_files              The concatenation of the lists 
#                               dependency_files_cpp and
#                               dependency_files_c
# test_dependency_files         The dependency files for the test in the
#                               current module.
# object_files_cpp              The list of C++ object files for the 
#                               current module.
# object_files_c                The list of C object files for the current
#                               module.
# object_files                  The concatenation of the two lists
#                               object_files_cpp and object_files_c.
# object_files_cpp_optimised    The list of optimised C++ object files for
#                               the current module.
# object_files_c_optimised      The list of optimised C object files for 
#                               the current module.
# object_files_optimised        The concatenation of the two lists 
#                               object_files_cpp_optimised and
#                               object_files_c_optimised.
# test_object_files             The list of C++ object files for the tests
#                               in the current module.
# test_object_files_optimised   The list of C object files for the tests in
#                               the current module.
# test_file                     Timestamp to mark the creation of the test 
#                               program for the current module.
# testop_file                   Timestamp to mark the creation of the 
#                               optimised test program for the current 
#                               module.
#
# alternative_gcc               Can be defined to specify the version number
#                               of a locally installed GCC to be used by the
#                               build system instead of the system compiler.
# alternative_boost             Can be defined to specify the version number
#                               of a locally installed Boost to be used by the
#                               build system instead of the system Boost.
# alternative_library_path      If alternative_boost is defined then the value
#                               of this variable specifies the location of the
#                               Boost link libraries directory.
#
# Test system variables
# ---------------------
# test_level                    By default has the value "basic" but can be
#                               redefined to either of the values "full" or
#                               "extensive". Specifies the level of testing
#                               carried out be (new) test system.
# error_file                    Specifies the location of the error log file
#                               for the test program of the current module.
# message_file                  Specifies the location of the messages log file
#                               for the test program of the current module.
# log_file                      Specifies the location of the log file for the
#                               test program of the current module.
# error_stream                  Specifies a stream for the streaming of error
#                               messages.
# message_stream                Specifies a stream for the streaming of non-
#                               error messages.
# log_stream                    Specifies a stream for the streaming of log
#                               messages.
# error_level                   Specifies verbosity level for error messages,
#                               by default 0 (lowest verbosity).
# message_level                 Specifies verbosity level for non-error 
#                               messages, default 0 (lowest verbosity).
# log_level                     Specifies verbosity level for log 
#                               messages, default 0 (lowest verbosity).
# error_lang                    Specifies language for error messages, by
#                               default equal to en_GB.
# message_lang                  Specifies language for non-error messages, by
#                               default equal to en_GB.
# log_lang                      Specifies language for log messages, by
#                               default equal to en_GB.
# test_parameters               Parameters to be passed to the test program for
#                               the current module.
#
# test_timestamp                Timestamp for the creation of test programs with
#                               the new test system.
# testop_timestamp              Timestamp for the creation of optimised test
#                               programs with the new test system.
#
#
# -------------------------------------------------------------------------------
# Make functions
# -------------------------------------------------------------------------------
#
# get-link-libraries            If the current module contains a file 
#                               Module.link_libraries then echo the contents
#                               of this file, otherwise echo the Make variable
#                               $(link_libraries).
# get-link-libraries_optimised  If the current module contains a file 
#                               Module.link_libraries_optimised then echo the 
#                               contents of this file, otherwise echo the Make 
#                               variable $(link_libraries).
