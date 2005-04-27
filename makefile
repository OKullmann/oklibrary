define last-element
$(word $(words $1),$1)
endef
srcdir := $(shell cd $(dir $(call last-element, $(MAKEFILE_LIST))); pwd)

include $(srcdir)/Buildsystem/makefile_recursive

