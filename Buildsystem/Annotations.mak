# Oliver Kullmann, 14.4.2005 (Swansea)

SHELL = /bin/bash
.SUFFIXES :

# The oklib-masterscript defines OKplatform.
ifndef OKplatform
  ifdef OKPLATFORM
    OKplatform := $(OKPLATFORM)
  else
    $(error Either OKplatform (a make-variable) or OKPLATFORM (an environment-variable) must be defined when calling this makefile (as the full directory path containing the OKplatform)!)
  endif
endif
ifndef OKconfiguration
  OKconfiguration := $(OKplatform)/.oklib/Configuration
endif

include $(OKconfiguration)/configuration_data.mak

# ######################################################################

include $(OKbuildsystem)/standardgoals.mak

include $(Annotations_dir)/definitions.mak

export

directories := $(aux_dir) $(latex_dir) $(doc_dir) $(dvi_dir)

# -------------------------------------------------------------------------

tex_files := $(wildcard $(Annotations_dir)/*.tex)
tex_files_bases := $(basename $(notdir $(tex_files)))
dvi_files := $(addprefix $(dvi_dir)/, $(addsuffix .dvi, $(tex_files_bases)))

# -------------------------------------------------------------------------

all force : $(dvi_files)

$(all_goals) :

# -------------------------------------------------------------------------

$(directories) :
	@mkdir -p $@

define run_latex
cd $(latex_dir); export TEXINPUTS=$(Annotations_dir):${TEXINPUTS}; export BIBINPUTS=$(Annotations_dir):${BIBINPUTS}; latex $<; bibtex $*.aux; latex $<; bibtex $*.aux; latex $<; latex $<; cp $*.dvi $@
endef

ifneq ($(firstword $(filter force, $(MAKECMDGOALS))), force)

$(dvi_files) : $(dvi_dir)/%.dvi : $(Annotations_dir)/%.tex | $(dvi_dir) $(latex_dir)
	$(run_latex)

else

.PHONY : FORCE

$(dvi_files) : $(dvi_dir)/%.dvi : $(Annotations_dir)/%.tex FORCE | $(dvi_dir) $(latex_dir)
	$(run_latex)

endif

# -------------------------------------------------------------------------

clean :
	- rm $(addprefix $(aux_dir)/, $(addsuffix .*, $(tex_files_bases)))

cleanall maintainer_clean : clean
	- rm $(dvi_files)
