# Oliver Kullmann, 16.8.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010, 2011, 2012 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the
# License, or any later version.

# Settings for building and using Git

all_extsrc_okl += git

git_recommended_version_number_okl ?= 1.7.8.4
git_other_versions_okl ?= 1.7.8

git_prefix_okl ?= git

git_recommended_okl := $(git_prefix_okl)-$(git_recommended_version_number_okl)
git_source_okl := $(ExternalSources)/sources/Git/$(git_recommended_okl)
git_source_ext_okl ?= .tar.bz2

git_base_build_dir_okl ?= $(ExternalSources_builds)/Git
git_build_dir_okl ?= $(git_base_build_dir_okl)/$(git_recommended_okl)

git_default_install_okl ?= local
# other possibility: system

git_base_installation_dir_okl ?= $(ExternalSources_installations)/Git
git_installation_dir_okl ?= $(git_base_installation_dir_okl)/$(git_recommended_version_number_okl)
git_system_install_directory_okl ?= /usr/local
ifeq ($(git_default_install_okl),local)
  git_install_directory_okl ?= $(git_installation_dir_okl)
  git_install_command_okl ?= make install
  git_call_okl ?= $(git_installation_dir_okl)/bin/git
  gitk_call_okl ?= $(git_installation_dir_okl)/bin/gitk
  git_upload_pack_okl ?= $(git_installation_dir_okl)/bin/git-upload-pack
  git_receive_pack_okl ?= $(git_installation_dir_okl)/bin/git-receive-pack
else
  git_install_directory_okl ?= $(git_system_install_directory)
  git_install_command_okl ?= sudo make install
  git_call_okl ?= git
  gitk_call_okl ?= gitk
  git_upload_pack_okl ?= git-upload-pack
  git_receive_pack_okl ?= git-receive-pack
endif

git_base_doc_dir_okl ?= $(ExternalSources_doc)/Git
git_doc_dir_okl ?= $(git_base_doc_dir_okl)/$(git_recommended_version_number_okl)
git_html_documentation_index_location_okl ?= $(git_doc_dir_okl)/Documentation/git.html
git_docu_page_okl ?= $(doc_dir)/doxygen_html/d7/d77/docus_2Git_8hpp.html

git_book_okl ?= $(ExternalSources)/sources/Git/book.pdf

git_homepage_url_okl := http://git-scm.com/
git_documentation_url_okl := http://git-scm.com/documentation

