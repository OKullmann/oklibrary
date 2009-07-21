# Oliver Kullmann, 17.8.2007 (Swansea)

General_options := -g
Optimisation_options := -O3 -DNDEBUG

CFLAGS ?= "-D MACHINE_BITS_OKL=$(machine_bits_okl)"

programs := OKsolver_2002 OKsolver_2002_lnk OKsolver_2002_osa OKsolver_2002_NTP OKsolver_2002_NLT OKsolver_2002_NTP_osa OKsolver_2002_NLT_osa OKsolver_2002_ALLSAT_NTP

source_libraries = 

link_libraries := -lm

