// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>
#include <type_traits>
#include <vector>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Ode.hpp"
#include "Stepper.hpp"
#include "Windows.hpp"
#include "InOut.hpp"

/*
TODOS:

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.6.7",
        "25.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferenetialEquations/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;
  using namespace Ode;
  using namespace Stepper;

  typedef X0Y0<float80, Euler1d> XY8_e;
  typedef X0Y0<float80, RK41d> XY8_r1;
  typedef X0Y0<float64, RK41d> XY6_r1;
  typedef X0Y0<float80, RK4> XY8_r;

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;

  {Euler1d<float80> E(0,0,[](float80, float80){return 0;}, [](float80){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
  }
  {Euler1d<float80> E(0,0,[](float80, float80){return 1;}, [](float80 x){return x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {Euler1d<float80> E(0,0,[](float80, float80){return 2;}, [](float80 x){return 2*x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -2);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {Euler1d<float80> E(0,0,[](float80 x, float80){return x;}, [](float80 x){return x*x/2;});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 2e14);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 1e14);
  }
  {Euler1d<float80> E(0,1,[](float80, float80 y){return y;}, [](float80 x){return FP::exp(x);});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 7e13);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 2e14);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 8e14);
  }
  {Euler1d<float80> E(0,1,[](float80, float80 y){return y;}, [](float80 x){return FP::exp(x);});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 7e12);
  }

  {Euler1d<float64> E(0,0,[](float64, float64){return 0;}, [](float64){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
  }
  {Euler1d<float64> E(0,0,[](float64, float64){return 1;}, [](float64 x){return x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 7e3);
  }
  {Euler1d<float64> E(0,0,[](float64, float64){return 2;}, [](float64 x){return 2*x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -2);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 7e3);
  }
  {Euler1d<float64> E(0,0,[](float64 x, float64){return x;}, [](float64 x){return x*x/2;});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 1e11);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 5e10);
  }
  {Euler1d<float64> E(0,1,[](float64, float64 y){return y;}, [](float64 x){return FP::exp(x);});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 0.4e11);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 9e10);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 4e11);
  }
  {Euler1d<float64> E(0,1,[](float64, float64 y){return y;}, [](float64 x){return FP::exp(x);});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 4e9);
  }


  {RK41d_80 E(0,0,[](float80, float80){return 0;}, [](float80){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
  }
  {RK41d_80 E(0,0,[](float80, float80){return 1;}, [](float80 x){return x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_80 E(0,0,[](float80, float80){return 2;}, [](float80 x){return 2*x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -2);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_80 E(0,0,[](float80 x, float80){return x;}, [](float80 x){return x*x/2;});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 1);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 1);
  }
  {RK41d_80 E(0,1,[](float80, float80 y){return y;}, [](float80 x){return FP::exp(x);});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 21);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 1);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 154);
  }
  {RK41d_80 E(0,1,[](float80, float80 y){return y;}, [](float80 x){return FP::exp(x);});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 63);
  }

  {RK41d_64 E(0,0,[](float64, float64){return 0;}, [](float64){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
  }
  {RK41d_64 E(0,0,[](float64, float64){return 1;}, [](float64 x){return x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() <= 1);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_64 E(0,0,[](float64, float64){return 2;}, [](float64 x){return 2*x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() <= 1);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_64 E(0,0,[](float64 x, float64){return x;}, [](float64 x){return x*x/2;});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 2);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() == 0);
  }
  {RK41d_64 E(0,1,[](float64, float64 y){return y;}, [](float64 x){return FP::exp(x);});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 35);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 7);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 38);
  }
  {RK41d_64 E(0,1,[](float64, float64 y){return y;}, [](float64 x){return FP::exp(x);});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 131);
  }


  {RK41d_auto<float80> E(0,0,[](float80){return 0;}, [](float80){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
  }
  {RK41d_auto<float80> E(0,0,[](float80){return 1;}, [](float80 x){return x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_auto<float80> E(0,0,[](float80){return 2;}, [](float80 x){return 2*x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == -2);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_auto<float80> E(0,1,[](float80 y){return y;}, [](float80 x){return FP::exp(x);});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 21);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 1);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 154);
  }
  {RK41d_auto<float80> E(0,1,[](float80 y){return y;}, [](float80 x){return FP::exp(x);});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 63);
  }
  {const auto F = [](float80 y){return y*y;};
   float80 c = 0;
   const auto sol = [&c](float80 x){return c / (1-c*x);};
   {RK41d_auto<float80> E(0,c,F,sol);
    E.steps(1);
    assert(E.x() == 1);
    assert(E.y() == 0);
    E.steps(-2);
    assert(E.x() == -1);
    assert(E.y() == 0);
   }
   {c = 2;
    RK41d_auto<float80> E(0,c,F,sol);
    E.steps(0.48L);
    assert(E.x() == 0.48L);
    assert(E.accuracy() <= 171'000);
    E.steps(-0.48L);
    assert(E.x() == 0);
    assert(E.accuracy() == 0);
    E.steps(-2);
    assert(E.x() == -2);
    assert(E.accuracy() <= 102);
    E.steps(-20);
    assert(E.x() == -22);
    assert(E.accuracy() <= 253);
   }
   {c = -2;
    RK41d_auto<float80> E(0,c,F,sol);
    E.steps(-0.48L);
    assert(E.x() == -0.48L);
    assert(E.accuracy() <= 170'000);
    E.steps(0.48L);
    assert(E.x() == 0);
    assert(E.accuracy() == 0);
    E.steps(2);
    assert(E.x() == 2);
    assert(E.accuracy() <= 102);
    E.steps(20);
    assert(E.x() == 22);
    assert(E.accuracy() <= 253);
   }
  }

  {RK41d_auto<float64> E(0,0,[](float64){return 0;}, [](float64){return 0;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.y() == 0);
   assert(E.accuracy() == 0);
  }
  {RK41d_auto<float64> E(0,0,[](float64){return 1;}, [](float64 x){return x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() <= 1);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_auto<float64> E(0,0,[](float64){return 2;}, [](float64 x){return 2*x;});
   E.step(1);
   assert(E.x() == 1);
   assert(E.y() == 2);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() <= 1);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK41d_auto<float64> E(0,1,[](float64 y){return y;}, [](float64 x){return FP::exp(x);});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 35);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 7);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 38);
  }
  {RK41d_auto<float64> E(0,1,[](float64 y){return y;}, [](float64 x){return FP::exp(x);});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 131);
  }

  typedef std::array<FP::float80, 1> vec80_1d;
  {RK4<float80,vec80_1d> E(0,{0},[](float80, vec80_1d){return vec80_1d{0};}, [](float80){return vec80_1d{0};});
   E.step(1);
   assert(E.x() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() == 0);
  }
  {RK4<float80,vec80_1d> E(0,{0},[](float80, vec80_1d){return vec80_1d{1};}, [](float80 x){return vec80_1d{x};});
   E.step(1);
   assert(E.x() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK4<float80,vec80_1d> E(0,{0},[](float80, vec80_1d){return vec80_1d{2};}, [](float80 x){return vec80_1d{2*x};});
   E.step(1);
   assert(E.x() == 1);
   assert(E.accuracy() == 0);
   E.step(-2);
   assert(E.x() == -1);
   assert(E.accuracy() == 0);
   E.steps(2);
   assert(E.x() == 1);
   assert(E.accuracy() <= 3e3);
  }
  {RK4<float80,vec80_1d> E(0,{0},[](float80 x, vec80_1d){return vec80_1d{x};}, [](float80 x){return vec80_1d{x*x/2};});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 1);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 1);
  }
  {RK4<float80,vec80_1d> E(0,{1},[](float80, vec80_1d y){return y;}, [](float80 x){return vec80_1d{FP::exp(x)};});
   E.steps(1);
   assert(E.x() == 1);
   assert(E.accuracy() <= 21);
   E.steps(1);
   assert(E.x() == 2);
   assert(E.accuracy() <= 1);
   E.steps(-3);
   assert(E.x() == -1);
   assert(E.accuracy() <= 154);
  }
  {RK4<float80,vec80_1d> E(0,{1},[](float80, vec80_1d y){return y;}, [](float80 x){return vec80_1d{FP::exp(x)};});
   E.steps(1,1e6);
   assert(E.x() == 1);
   assert(E.accuracy() <= 63);
  }

  {RK4<float80,vec80_2d> E(0,{0,1},[](float80 x, vec80_2d y){return vec80_2d{2, 2*x*y[1]};}, [](float80 x){return vec80_2d{2*x, FP::exp(x*x)};});
   E.steps(2, 1e5);
   assert(E.x() == 2);
   assert(E.accuracy() <= 11800);
  }
  {RK4<float80,vec80_3d> E(0,{2,1,-1},[](float80 x, vec80_3d y){return vec80_3d{FP::cos(x), 2*x*y[1], 3*x*x};}, [](float80 x){return vec80_3d{FP::sin(x)+2, FP::exp(x*x), x*x*x-1};});
   E.steps(2, 1e5);
   assert(E.x() == 2);
   assert(E.accuracy() <= 180);
   RK4 E2(E);
   E2.step(3);
   E2.steps(0);
   E.steps(3,1);
   assert(E.x() == E2.x());
   assert(E.y() == E2.y());
  }

  {const auto F = [](float80, float80 y){return y*y;};
   float80 c = 1;
   const auto sol = [c](float80 x){return c / (1-c*x);};
   XY8_r1 E(0,c,F,sol), E2(0,c,F,sol), E3(E2), E4(E2), E5(E4), E6(E), E7(E6);

   {E.interval(0,true,1,false, 1,1e4L);
    assert(E.x() == 0);
    assert(E.y() == 1);
    assert((E.points() == XY8_r1::points_vt{{0,1}}));
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == 0);
    assert(S.xmax[0] == 0);
    assert(S.ymin[1] == 1);
    assert(S.ymax[1] == 1);
    assert(S.ymean == 1);
    assert(S.ysd == 0);
    assert(Sa.ymin[1] == 0);
    assert(Sa.ymax[1] == 0);
    assert(Sa.ymean == 0);
    assert(Sa.ysd == 0);}

   {E2.interval(0,false,0.5L,true, 1,1e4L);
    assert(E2.x() == 0.5L);
    assert(E2.accuracy() <= 118);
    assert(E2.points().size() == 1);
    const auto S = E2.stats();
    const auto Sa = E2.stats_acc();
    assert(S.xmin[0] == 0.5);
    assert(S.xmax[0] == 0.5);
    assert(S.ymin[1] == E2.y());
    assert(S.ymax[1] == E2.y());
    assert(S.ymean == E2.y());
    assert(S.ysd == 0);
    assert(Sa.ymin[1] == E2.accuracy());
    assert(Sa.ymax[1] == E2.accuracy());
    assert(Sa.ymean == E2.accuracy());
    assert(Sa.ysd == 0);}

   {E3.interval(0,false,1,false, 2,1e4L);
    assert(E3.x() == 0.5L);
    assert(E3.accuracy() <= 118);
    assert(E3.points().size() == 1);
    const auto S = E3.stats();
    const auto Sa = E3.stats_acc();
    assert(S.xmin[0] == 0.5);
    assert(S.xmax[0] == 0.5);
    assert(S.ymin[1] == E3.y());
    assert(S.ymax[1] == E3.y());
    assert(S.ymean == E3.y());
    assert(S.ysd == 0);
    assert(Sa.ymin[1] == E3.accuracy());
    assert(Sa.ymax[1] == E3.accuracy());
    assert(Sa.ymean == E3.accuracy());
    assert(Sa.ysd == 0);}

   {E4.interval(-1,true,0.5,true, 1,1e4L);
    assert(E4.x() == 0.5L);
    assert(E4.accuracy() <= 118);
    assert(E4.points().size() == 2);
    const auto S = E4.stats();
    const auto Sa = E4.stats_acc();
    assert(S.xmin[0] == -1);
    assert(S.xmax[0] == 0.5);
    assert(accuracy(0.5, S.ymin[1]) <= 30);
    assert(S.ymax[1] == E4.y());
    assert(accuracy(1.25, S.ymean) <= 52);
    assert(accuracy(0.75, S.ysd) <= 133);
    assert(Sa.ymin[1] == 30);
    assert(Sa.ymax[1] == 118);
    assert(Sa.ymean == midpoint(30,118));
    assert(Sa.ysd == 44);}

   {E5.interval(-0.1L,true,0.5,true, 2,1e4L);
    assert(E5.x() == 0.5L);
    assert(E5.accuracy() <= 54);
    assert(E5.points().size() == 3);
    const auto S = E5.stats();
    const auto Sa = E5.stats_acc();
    assert(S.xmin[0] == -0.1L);
    assert(S.xmax[0] == 0.5);
    assert(accuracy(10.0L/11, S.ymin[1]) <= 32);
    assert(S.ymax[1] == E5.y());
    assert(accuracy(61.0L/44, S.ymean) <= 37);
    assert(accuracy(sqrt(201.0L)/11/cb(Sqr2), S.ysd) <= 60);
    assert(Sa.ymin[1] == 31);
    assert(Sa.ymax[1] == 54);
    assert(Sa.ymean == 127.0L/3);
    assert(Sa.ysd == sqrt(794.0L)/3);}

   {E6.interval(-1,true,0.1L,true, 2,1e4L);
    assert(E6.x() == -1);
    assert(E6.accuracy() <= 4);
    assert(E6.points().size() == 3);
    const auto S = E6.stats();
    const auto Sa = E6.stats_acc();
    assert(S.xmin[0] == -1);
    assert(S.xmax[0] == 0.1L);
    assert(accuracy(0.5L, S.ymin[1]) <= 4);
    assert(accuracy(10.0L/9, S.ymax[1]) <= 52);
    assert(accuracy(1201.0L/1566, S.ymean) <= 41);
    assert(accuracy(11*sqrt(661.0L)/783/Sqr2, S.ysd) <= 92);
    assert(Sa.ymin[1] == 4);
    assert(Sa.ymax[1] == 52);
    assert(Sa.ymean == 77.0L/3);
    assert(Sa.ysd == sqrt(3554.0L)/3);}

   {E7.interval(-1,true,0.5L,true, 3,1e4L);
    assert(E7.x() == 0.5);
    assert(E7.accuracy() <= 118);
    assert(E7.points().size() == 4);
    const auto S = E7.stats();
    const auto Sa = E7.stats_acc();
    assert(S.xmin[0] == -1);
    assert(S.xmax[0] == 0.5L);
    assert(accuracy(0.5L, S.ymin[1]) <= 25);
    assert(accuracy(2, S.ymax[1]) <= 118);
    assert(accuracy(25.0L/24, S.ymean) <= 25);
    assert(accuracy(sqrt(65.0L)/8/sqrt(3.0L), S.ysd) <= 104);
    assert(Sa.ymin[1] == 0);
    assert(Sa.ymax[1] == 118);
    assert(Sa.ymean == 149.0L/4);
    assert(Sa.ysd == sqrt(36139.0L)/4);}

   {E.interval(0,true,1,false, 2,1e4L);
    assert(E.x() == 0.5L);
    assert(E.accuracy() <= 118);
    assert(E.points().size() == 2);
    assert((E.points().front() == XY8_r1::point_t{0,1}));
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == 0);
    assert(S.xmax[0] == 0.5L);
    assert(S.ymin[1] == 1);
    assert(S.ymax[1] == E.y());
    assert(S.ymean == (1 + E.y())/2);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] == 0);
    assert(Sa.ymax[1] == E.accuracy());
    assert(Sa.ymean == E.accuracy() / 2);
    assert(Sa.ysd != 0);}

   {E.interval(-1.5,true,0.5,true, 2,1e4L);
    assert(E.x() == -1.5L);
    assert(E.accuracy() <= 78);
    assert(E.points().size() == 3);
    assert(E.points()[0].x == -1.5L);
    assert(E.points()[1].x == -0.5L);
    assert(E.points()[2].x == 0.5L);
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == -1.5L);
    assert(S.xmax[0] == 0.5L);
    assert(S.ymin[1] == E.points()[0].y);
    assert(S.ymax[1] == E.points()[2].y);
    assert(S.ymean != 0);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] != 0);
    assert(Sa.ymax[1] == 118);
    assert(Sa.ymean > 0 and Sa.ymean < 118);
    assert(Sa.ysd != 0);}

   {E.interval(-1.5,false,0,false, 3,1e4L);
    assert(E.x() == -0.5L);
    assert(E.accuracy() <= 104);
    assert(E.points().size() == 2);
    assert(E.points()[0].x == -1);
    assert(E.points()[1].x == -0.5L);
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == -1);
    assert(S.xmax[0] == -0.5L);
    assert(S.ymin[1] == E.points()[0].y);
    assert(S.ymax[1] == E.points()[1].y);
    assert(S.ymean == (E.points()[0].y + E.points()[1].y) / 2);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] != 0);
    assert(Sa.ymax[1] == 104);
    assert(Sa.ymean > 0 and Sa.ymean < 104);
    assert(Sa.ysd != 0);}
  }

  {const auto F = [](float80 x, float80){return 1/x;};
   const auto sol = [](float80 x){return FP::log(x);};
   XY8_r1 E(1,0,F,sol);

   {E.interval(0,false,1,false, 10, 1e4);
    assert(accuracy(0.1L, E.x()) <= 5);
    assert(E.accuracy() <= 16);
    assert(E.points().size() == 9);
    assert(E.points()[0].x == E.x());
    assert(E.points()[8].x == 0.9L);
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == E.x());
    assert(S.xmax[0] == 0.9L);
    assert(S.ymin[1] == E.y());
    assert(accuracy(FP::log(0.9L), S.ymax[1]) <= 25);
    assert(S.ymean < S.ymax[1]);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] != 0);
    assert(Sa.ymax[1] <= 55);
    assert(Sa.ysd != 0);}

   {E.interval(0,false,10,true, 10, 1e5);
    assert(E.x() == 10);
    assert(E.accuracy() <= 138);
    assert(E.points().size() == 10);
    assert(E.points()[0].x == 1);
    assert(E.points()[9].x == 10);
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == 1);
    assert(S.xmax[0] == 10);
    assert(accuracy(0,S.ymin[1],PrecZ::eps) <= 149);
    assert(S.ymax[1] == E.y());
    assert(S.ymean < S.ymax[1]);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] != 0);
    assert(Sa.ymax[1] <= 381);
    assert(Sa.ysd != 0);}
  }

  {const auto F = [](float64 x, float64 y){return x*y;};
   const auto sol = [](float64 x){return FP::exp(x*x/2);};
   XY6_r1 E(0,1,F,sol);

   {E.interval(-10,true,10,true, 20, 1e4);
    assert(E.x() == 10);
    assert(E.accuracy() <= 910);
    assert(E.points().size() == 21);
    assert(E.points()[20].x == E.x());
    assert(E.points()[0].x == -10);
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == -10);
    assert(S.xmax[0] == E.x());
    assert(S.ymin[1] == 1);
    assert(accuracy_64(sol(-10), S.ymax[1]) <= 910);
    assert(S.ymean < S.ymax[1]);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] == 0);
    assert(Sa.ymax[1] == 910);
    assert(Sa.ysd != 0);}

   {E.interval(-10,true,10,true, 20, 1e4);
    assert(E.x() == -10);
    assert(E.accuracy() <= 910);
    assert(E.points().size() == 21);
    assert(E.points()[20].x == 10);
    assert(E.points()[0].x == E.x());
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == E.x());
    assert(S.xmax[0] == 10);
    assert(S.ymin[1] == 1);
    assert(accuracy_64(sol(-10), S.ymax[1]) <= 910);
    assert(S.ymean < S.ymax[1]);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] == 0);
    assert(Sa.ymax[1] == 910);
    assert(Sa.ysd != 0);}

   {E.interval(-10,true,-5,true, 10, 1e4);
    assert(E.x() == -5);
    assert(E.accuracy() <= 774);
    assert(E.points().size() == 11);
    assert(E.points()[0].x == -10);
    assert(E.points()[1].x == -9.5);
    assert(E.points()[10].x == E.x());
    const auto S = E.stats();
    const auto Sa = E.stats_acc();
    assert(S.xmin[0] == -10);
    assert(S.xmax[0] == E.x());
    assert(accuracy_64(sol(-5), S.ymin[1]) <= 774);
    assert(accuracy_64(sol(-10), S.ymax[1]) <= 910);
    assert(S.ymean < S.ymax[1]);
    assert(S.ysd != 0);
    assert(Sa.ymin[1] == 774);
    assert(Sa.ymax[1] == 1517);
    assert(Sa.ymean < 1517 and Sa.ymean > 774);
    assert(Sa.ysd != 0);}
  }

  {XY8_e s(0,1,[](float80, float80 y){return y;}, [](float80 x){return FP::exp(x);});
   s.interval(0,true, 2,true, 10, 1e4);
  }
  {const auto F = [](float80, float80 y){return y*y;};
   float80 c = 1;
   const auto sol = [c](float80 x){return c / (1-c*x);};
   XY8_r1 s(0,1,F,sol);
   s.interval(0,true,1,false, 1,1e4L);
  }
  {typedef XY8_r::x_t x_t;
   typedef XY8_r::y_t y_t;
   assert((std::is_same_v<x_t,float80>));
   assert((std::is_same_v<y_t,std::vector<float80>>));
   typedef XY8_r::F_t F_t;
   typedef XY8_r::f_t f_t;
   const F_t F = [](const x_t, const y_t y){return y_t{y[1],y[0]};};
   const f_t sol = [](const x_t x){return y_t{exp(x), exp(x)};};
   XY8_r s(0,sol(0), F,sol);
   assert(s.size == 2);
   s.interval(0,true, 10,true, 10,1e4);
   assert(s.x() == 10);
   assert(s.accuracy() <= 54);
   assert(s.a() == 0);
   assert(s.b() == 10);
   assert(s.left_included());
   assert(s.right_included());
   assert(s.points().size() == 11);

   typedef XY8_r::stats_t stats_t;
   typedef stats_t::point_t point_t;
   const stats_t S0 = s.stats(0);
   assert((S0.xmin == point_t{0,1}));
   assert((S0.xmax[0] == 10));
   assert(accuracymax(point_t{10,FP::exp(10)}, S0.xmax) <= 54);
   assert((S0.ymin == point_t{0,1}));
   assert(accuracymax(point_t{10,FP::exp(10)}, S0.ymax) <= 54);
   assert(S0.ymean < S0.ymax[1]);
   assert(S0.ysd != 0);
   const stats_t S1 = s.stats(1);
   assert((S1.xmin == point_t{0,1}));
   assert((S1.xmax == point_t{10, s.y()[1]}));
   
   const stats_t S0a = s.stats_acc(0);
   assert(s.points().size() == s.accuracies().size());
   assert((s.accuracies().back() == XY8_r::point_t{10,{54,54}}));
   assert((S0a.xmin == point_t{0,0}));
   assert((S0a.xmax == point_t{10,54}));
   const stats_t S1a = s.stats_acc(1);
   assert(s.points().size() == s.accuracies().size());
   assert((S1a.xmin == point_t{0,0}));
   assert((S1a.xmax == point_t{10,54}));
  }
}
