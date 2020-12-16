#include <iostream>
#include <string>

#include <cstdio>

#include <gmp.h>
#include <mpfr.h>

int main () {
  std::printf("MPFR library: %-12s\nMPFR header:  %s (based on %d.%d.%d)\n",
          mpfr_get_version (), MPFR_VERSION_STRING, MPFR_VERSION_MAJOR,
          MPFR_VERSION_MINOR, MPFR_VERSION_PATCHLEVEL);
  std::printf("GMP %-12s\nversion-details %d %d %d\n",
       gmp_version,
       __GNU_MP_VERSION, __GNU_MP_VERSION_MINOR, __GNU_MP_VERSION_PATCHLEVEL);

  mpz_t n;
  mpz_init(n);
  mpz_fac_ui(n, 100);
  std::cout << "\n100!=";
  gmp_printf("%Zd\n", n);

  std::cout << "\nMPFR_PREC_MIN=" << MPFR_PREC_MIN << ", MPFR_PREC_MAX=" << MPFR_PREC_MAX << "\n";
  constexpr long long unsigned prec = 30;
  constexpr mpfr_prec_t uprec = 4 * prec;
  std::cout << "Underlying binary precision: " << uprec << "\n";
  mpfr_t x;
  mpfr_init2(x, uprec);
  mpfr_set_si(x,2,MPFR_RNDN);
  mpfr_log(x,x, MPFR_RNDN);
  std::cout << "ln(2)=";
  const std::string format = "%." + std::to_string(prec) + "Re\n";
  mpfr_printf(format.c_str(), x);
}
