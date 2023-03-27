// Oliver Kullmann, 27.3.2023 (Swansea)
/* Copyright 2023 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>

#include "SystemCalls.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "27.3.2023",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/SystemSpecifics/TestSystemCalls.cpp",
        "GPL v3"};

  using namespace SystemCalls;

  template <class X>
  constexpr bool eqp(const X& lhs, const X& rhs) noexcept {
    return lhs == rhs;
  }
}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {const ReturnValue rv = esystem("","","","");
   assert(eqp(rv, {0}));
   assert(rv.s == ExitStatus::normal);
   assert(rv.val == 0);
   assert(not rv.continued);
   assert(rv == esystem("true","","",""));
  }
  {const ReturnValue rv = esystem("false","","","");
   assert(rv.s == ExitStatus::normal);
   assert(rv.val == 1);
   assert(not rv.continued);
  }

  {const EReturnValue er = esystem("","");
   assert(eqp(er.rv, {0}));
   assert(er.out.empty());
   assert(er.err.empty());
  }
  {const EReturnValue er = esystem("false","");
   assert(er.rv == esystem("false","","",""));
   assert(er.out.empty());
   assert(er.err.empty());
  }
  {const EReturnValue er = esystem("echo \"ABCD\"", "");
   assert(eqp(er.rv, {0}));
   assert(er.out == "ABCD\n");
   assert(er.err.empty());
  }
  {const EReturnValue er = esystem("cat -", "echo \"ABCD\"", true);
   assert(eqp(er.rv, {0}));
   assert(er.out == "ABCD\n");
   assert(er.err.empty());
  }

  {Popen P("", "", "");
   assert(P.command.empty());
   assert(P.cout.empty());
   assert(P.cerr.empty());
   assert(not P.delete_on_exit());
   assert(P.cin_valid());
   assert(P.transfer([](std::FILE*){}) == esystem("","","",""));
   assert(not P.cin_valid());
  }
  {Popen P("true", "", "");
   assert(P.command == "true");
   assert(P.cout.empty());
   assert(P.cerr.empty());
   assert(not P.delete_on_exit());
   assert(P.cin_valid());
   assert(P.transfer([](std::FILE*){}) == esystem("","","",""));
   assert(not P.cin_valid());
  }
  {Popen P("false", "", "");
   assert(not P.delete_on_exit());
   assert(P.cin_valid());
   assert(P.transfer([](std::FILE*){}) == esystem("false","","",""));
   assert(not P.cin_valid());
  }
  {Popen P("");
   assert(P.delete_on_exit());
   assert(P.cin_valid());
   assert(P.transfer([](std::FILE*){}) == esystem("","","",""));
   assert(not P.cin_valid());
  }
  {Popen P("true");
   assert(P.delete_on_exit());
   assert(P.cin_valid());
   assert(P.transfer([](std::FILE*){}) == esystem("","","",""));
   assert(not P.cin_valid());
  }
  {Popen P("false");
   assert(P.delete_on_exit());
   assert(P.cin_valid());
   assert(P.transfer([](std::FILE*){}) == esystem("false","","",""));
   assert(not P.cin_valid());
  }
  {Popen P("cat -");
   const stringref_put cin("echo 777");
   const auto res = P.etransfer(cin);
   assert(res.rv == ReturnValue(0));
   assert(res.out == "echo 777");
   assert(res.err.empty());
  }
  {Popen P("cat -");
   struct {
     void operator()(std::FILE* const fp) const {fputs("X345\n", fp);}
   } cin;
   const auto res = P.etransfer(cin);
   assert(res.rv == ReturnValue(0));
   assert(res.out == "X345\n");
   assert(res.err.empty());
  }
  {Popen P("cat -");
   struct {
     void operator()(std::FILE* const fp) const {fputs("X777", fp);}
   } cin0;
   const put_cin_t cin(cin0);
   const auto res = P.etransfer(cin);
   assert(res.rv == ReturnValue(0));
   assert(res.out == "X777");
   assert(res.err.empty());
  }

}
