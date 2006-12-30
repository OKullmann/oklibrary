external_sources_doc_base_dir := $(ExternalSources)/doc

boost_doc_dir := $(external_sources_doc_base_dir)/Boost
gcc_doc_dir := $(external_sources_doc_base_dir)/Gcc
mhash_doc_dir := $(external_sources_doc_base_dir)/Mhash
postgresql_doc_dir := $(external_sources_doc_base_dir)/Postgresql
doxygen_doc_dir := $(external_sources_doc_base_dir)/Doxygen

doc_directories := $(boost_doc_dir) \
                   $(gcc_doc_dir) \
                   $(mhash_doc_dir) \
                   $(postgresql_doc_dir) \
                   $(doxygen_doc_dir)

boost_doc : | $(boost_doc_dir)
	- $(call unarchive,$(boost_recommended_package_name),$(boost_doc_dir))

gcc_doc : | $(gcc_doc_dir)
	- $(call unarchive,$(gcc_recommended),$(gcc_doc_dir))

mhash_doc : | $(mhash_doc_dir)
	- $(call unarchive,$(mhash_recommended),$(mhash_doc_dir))

postgresql_doc : | $(postgresql_doc_dir)
	- $(call unarchive,$(postgresql_recommended),$(postgresql_doc_dir))

doxygen_doc : | $(doxygen_doc_dir)
	$(call unarchive,$(doxygen_recommended_package_name),$(doxygen_doc_dir))
	cd $(doxygen_doc_dir)/$(doxygen_recommended); $(postcondition) \
	sh ./configure; $(postcondition) \
	make; $(postcondition)\
	make docs; $(postcondition) \
	make pdf; $(postcondition) \

doc : boost_doc gcc_doc mhash_doc postgresql_doc doxygen_doc

$(doc_directories) : % : | $(external_sources_doc_base_dir)
	mkdir $@

$(external_sources_doc_base_dir) :
	mkdir $(external_sources_doc_base_dir)