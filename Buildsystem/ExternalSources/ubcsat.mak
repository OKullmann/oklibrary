# Matthew Lewsey, 9.11.2006 (Swansea)
# filename : Buildsystem/ExternalSources/ubcsat.mak


# ##################################
# Directory Structure
# ##################################

ubcsat-base-directory := $(ExternalSources)/Ubcsat
ubcsat-extract-directory := $(ubcsat-base-directory)/1-0-0
ubcsat-lib-directory := $(ubcsat-base-directory)/1-0-0/lib
ubcsat-bin-directory := $(ubcsat-base-directory)/1-0-0/bin
ubcsat-src-directory := $(ubcsat-base-directory)/1-0-0/src
ubcsat-tmp-src-directory := $(ubcsat-base-directory)/1-0-0/tmp

ubcsat-directories := $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-lib-directory) $(ubcsat-bin-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory)

ubcsat_names := adaptnovelty algorithms gsat gsat-tabu gwsat hsat hwsat irots mt19937ar mylocal novelty parameters reports rnovelty rots samd saps ubcsat ubcsat-help ubcsat-internal ubcsat-io ubcsat-mem ubcsat-reports ubcsat-time ubcsat-triggers walksat walksat-tabu

ubcsat_c_files := $(addsuffix .c, $(ubcsat_names))
ubcsat_c_files_paths := $(addprefix $(ubcsat-tmp-src-directory)/, $(ubcsat_c_files))

ubcsat-changed_dir := $(OKsystem)/Transitional/LocalSearch/Ubcsat/corrected
ubcsat_changed_files := $(wildcard $(ubcsat-changed_dir)/*.h) $(wildcard $(ubcsat-changed_dir)/*.c)

$(ubcsat-directories) : % : 
	mkdir $@

ubcsat_o_files := $(addsuffix .o, $(ubcsat_names))
paths := $(addprefix $(ubcsat-lib-directory)/, $(ubcsat_o_files))

# #################################
# The Targets
# #################################

$(ubcsat-extract-directory)/tag : | $(ubcsat-base-directory) $(ubcsat-extract-directory) $(ubcsat-src-directory) $(ubcsat-tmp-src-directory)
	$(call unarchive,ubcsat-1-0-0,$(ubcsat-extract-directory),src)
	dos2unix $(ubcsat-src-directory)/*.c $(ubcsat-src-directory)/*.h
	cp $(ubcsat-src-directory)/* $(ubcsat-tmp-src-directory)
	cp -f $(ubcsat_changed_files) $(ubcsat-tmp-src-directory)
	touch $@

ubcsat : $(ubcsat-extract-directory)/tag $(ubcsat-bin-directory)/ubcsat $(ubcsat-lib-directory)/libubcsat.a clean_tmp

$(ubcsat-bin-directory)/ubcsat : | $(ubcsat-bin-directory)
	gcc -Wall -O3 -o $(ubcsat-bin-directory)/ubcsat $(ubcsat_c_files_paths) -lm

$(paths) : $(ubcsat-lib-directory)/%.o : $(ubcsat-tmp-src-directory)/%.c | $(ubcsat-installation-directory) $(ubcsat-lib-directory)
	gcc -Wall -O3 -c $< -o $@ -DALTERNATEMAIN

$(ubcsat-lib-directory)/libubcsat.a : $(paths)
	$(AR) $(ARFLAGS) $@ $^

.PHONY : clean_tmp cleanallubcsat

clean_tmp :
	rm -rf $(ubcsat-tmp-src-directory)

cleanallubcsat :
	-rm -rf $(ubcsat-base-directory)
