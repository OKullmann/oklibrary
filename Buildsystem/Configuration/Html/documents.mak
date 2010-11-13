# Oliver Kullmann, 11.9.2007 (Swansea)
# Copyright 2007, 2008, 2009, 2010 Oliver Kullmann
# This file is part of the OKlibrary. OKlibrary is free software; you can redistribute 
# it and/or modify it under the terms of the GNU General Public License as published by
# the Free Software Foundation and included in this library; either version 3 of the 
# License, or any later version.

# Configuration data regarding documents

oklibrary_full_documentation := FullDocumentation
oklibrary_dvi_documents ?= $(oklibrary_full_documentation)
OKlibrary_full_documentation_dvi_location ?= $(documents_html_dir)/$(oklibrary_full_documentation).dvi

accu2006_talk_location ?= $(documents_html_dir)/200604Oxford.pdf

epsrc_faststream_pdf_location ?= $(documents_html_dir)/EPSRCFastStreamCase.pdf
epsrc_6finalreport_pdf_location ?= $(documents_html_dir)/EPSRC_6FinalReport.pdf
epsrc_finalreport_pdf_location ?= $(documents_html_dir)/EPSRCFinalReport.pdf

baltimore2008_talk_location ?= $(documents_html_dir)/200803Baltimore.pdf

oklibrary_report2009_location ?= $(documents_html_dir)/CSR1-2009.pdf

greentao_pcv2010_talk_location ?= $(documents_html_dir)/RamseyTheory/2010_Swansea05.pdf

