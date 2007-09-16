# Oliver Kullmann, 14.9.2007 (Swansea)

# Configuration data for the release process

package_build_script ?= $(OKbuildsystem)/ReleaseProcess/CreatePackage
packages_dir ?= $(system_directories)/packages

release_history_template ?= $(OKbuildsystem)/Configuration/ReleaseProcess/ReleaseHistory
release_history_output ?= $(local_html_dir)/ReleaseHistory

readme_file ?= $(OKbuildsystem)/ReleaseProcess/README

