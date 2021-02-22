// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

#include <iostream>

#include <cassert>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>

#include "Ode.hpp"

/*
TODOS:

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.3.3",
        "21.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferenetialEquations/Test.cpp",
        "GPL v3"};

  using namespace FloatingPoint;
  using namespace Ode;

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
   const auto sol = [&c](float80 x){return c / (1-c*x);};
   typedef RK41d<float80> RK;
   RK E(0,c,F,sol);

   E.interval(0,true,1,false,1, 1e4L);
   assert(E.x() == 0);
   assert(E.y() == 1);
   assert((E.points() == RK::points_vt{{0,1}}));
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == 0);
   assert(E.xmax() == 0);
   assert(E.ymin() == 1);
   assert(E.ymax() == 1);
   assert(E.ymean() == 1);
   assert(E.ysd() == 0);
   assert(E.accmin() == 0);
   assert(E.accmax() == 0);
   assert(E.accmean() == 0);
   assert(E.accsd() == 0);

   E.interval(0,true,1,false,2, 1e4L);
   assert(E.x() == 0.5L);
   assert(E.accuracy() <= 118);
   assert(E.points().size() == 2);
   assert((E.points().front() == RK::point_t{0,1}));
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == 0);
   assert(E.xmax() == 0.5L);
   assert(E.ymin() == 1);
   assert(E.ymax() == E.y());
   assert(E.ymean() == (1 + E.y())/2);
   assert(E.ysd() != 0);
   assert(E.accmin() == 0);
   assert(E.accmax() == E.accuracy());
   assert(E.accmean() == E.accuracy() / 2);
   assert(E.accsd() != 0);

   E.interval(-1.5,true,0.5,true,2, 1e4L);
   assert(E.x() == -1.5L);
   assert(E.accuracy() <= 78);
   assert(E.points().size() == 3);
   assert(E.points()[0][0] == -1.5L);
   assert(E.points()[1][0] == -0.5L);
   assert(E.points()[2][0] == 0.5L);
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == -1.5L);
   assert(E.xmax() == 0.5L);
   assert(E.ymin() == E.points()[0][1]);
   assert(E.ymax() == E.points()[2][1]);
   assert(E.ymean() != 0);
   assert(E.ysd() != 0);
   assert(E.accmin() != 0);
   assert(E.accmax() == 118);
   assert(E.accmean() > 0 and E.accmean() < 118);
   assert(E.accsd() != 0);

   E.interval(-1.5,false,0,false,3, 1e4L);
   assert(E.x() == -0.5L);
   assert(E.accuracy() <= 104);
   assert(E.points().size() == 2);
   assert(E.points()[0][0] == -1);
   assert(E.points()[1][0] == -0.5L);
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == -1);
   assert(E.xmax() == -0.5L);
   assert(E.ymin() == E.points()[0][1]);
   assert(E.ymax() == E.points()[1][1]);
   assert(E.ymean() == (E.points()[0][1] + E.points()[1][1]) / 2);
   assert(E.ysd() != 0);
   assert(E.accmin() != 0);
   assert(E.accmax() == 104);
   assert(E.accmean() > 0 and E.accmean() < 104);
   assert(E.accsd() != 0);
  }

  {const auto F = [](float80 x, float80){return 1/x;};
   const auto sol = [](float80 x){return FP::log(x);};
   typedef RK41d<float80> RK;
   RK E(1,0,F,sol);

   E.interval(0,false,1,false, 10, 1e4);
   assert(accuracy(0.1L, E.x()) <= 5);
   assert(E.accuracy() <= 16);
   assert(E.points().size() == 9);
   assert(E.points()[0][0] == E.x());
   assert(E.points()[8][0] == 0.9L);
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == E.x());
   assert(E.xmax() == 0.9L);
   assert(E.ymin() == E.y());
   assert(accuracy(FP::log(0.9L), E.ymax()) <= 25);
   assert(E.ymean() < E.ymax());
   assert(E.ysd() != 0);
   assert(E.accmin() != 0);
   assert(E.accmax() <= 55);
   assert(E.accsd() != 0);

   E.interval(0,false,10,true, 10, 1e5);
   assert(E.x() == 10);
   assert(E.accuracy() <= 138);
   assert(E.points().size() == 10);
   assert(E.points()[0][0] == 1);
   assert(E.points()[9][0] == 10);
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == 1);
   assert(E.xmax() == 10);
   assert(accuracy(0,E.ymin(),PrecZ::eps) <= 149);
   assert(E.ymax() == E.y());
   assert(E.ymean() < E.ymax());
   assert(E.ysd() != 0);
   assert(E.accmin() != 0);
   assert(E.accmax() <= 381);
   assert(E.accsd() != 0);
  }

  {const auto F = [](float64 x, float64 y){return x*y;};
   const auto sol = [](float64 x){return FP::exp(x*x/2);};
   typedef RK41d<float64> RK;
   RK E(0,1,F,sol);

   E.interval(-10,true,10,true, 20, 1e4);
   assert(E.x() == 10);
   assert(E.accuracy() <= 910);
   assert(E.points().size() == 21);
   assert(E.points()[20][0] == E.x());
   assert(E.points()[0][0] == -10);
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == -10);
   assert(E.xmax() == E.x());
   assert(E.ymin() == 1);
   assert(accuracy_64(sol(-10), E.ymax()) <= 910);
   assert(E.ymean() < E.ymax());
   assert(E.ysd() != 0);
   assert(E.accmin() == 0);
   assert(E.accmax() == 910);
   assert(E.accsd() != 0);

   E.interval(-10,true,10,true, 20, 1e4);
   assert(E.x() == -10);
   assert(E.accuracy() <= 910);
   assert(E.points().size() == 21);
   assert(E.points()[20][0] == 10);
   assert(E.points()[0][0] == E.x());
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == E.x());
   assert(E.xmax() == 10);
   assert(E.ymin() == 1);
   assert(accuracy_64(sol(-10), E.ymax()) <= 910);
   assert(E.ymean() < E.ymax());
   assert(E.ysd() != 0);
   assert(E.accmin() == 0);
   assert(E.accmax() == 910);
   assert(E.accsd() != 0);

   E.interval(-10,true,-5,true, 10, 1e4);
   assert(E.x() == -5);
   assert(E.accuracy() <= 774);
   assert(E.points().size() == 11);
   assert(E.points()[0][0] == -10);
   assert(E.points()[1][0] == -9.5);
   assert(E.points()[10][0] == E.x());
   E.update_stats();
   E.update_accuracies();
   assert(E.xmin() == -10);
   assert(E.xmax() == E.x());
   assert(accuracy_64(sol(-5), E.ymin()) <= 774);
   assert(accuracy_64(sol(-10), E.ymax()) <= 910);
   assert(E.ymean() < E.ymax());
   assert(E.ysd() != 0);
   assert(E.accmin() == 774);
   assert(E.accmax() == 1517);
   assert(E.accmean() < 1517 and E.accmean() > 774);
   assert(E.accsd() != 0);
  }
}