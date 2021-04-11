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
#include "Stepper.hpp"

/*
TODOS:

*/


namespace {

  const Environment::ProgramInfo proginfo{
        "0.5.0",
        "11.4.2021",
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
   XY8_r1 E(0,c,F,sol), E2(0,c,F,sol), E3(E2), E4(E2), E5(E4), E6(E), E7(E6);

   E.interval(0,true,1,false, 1,1e4L);
   assert(E.x() == 0);
   assert(E.y() == 1);
   assert((E.points() == XY8_r1::points_vt{{0,1}}));
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

   E2.interval(0,false,0.5L,true, 1,1e4L);
   assert(E2.x() == 0.5L);
   assert(E.accuracy() <= 118);
   assert(E2.points().size() == 1);
   E2.update_stats();
   E2.update_accuracies();
   assert(E2.xmin() == 0.5);
   assert(E2.xmax() == 0.5);
   assert(E2.ymin() == E2.y());
   assert(E2.ymax() == E2.y());
   assert(E2.ymean() == E2.y());
   assert(E2.ysd() == 0);
   assert(E2.accmin() == E2.accuracy());
   assert(E2.accmax() == E2.accuracy());
   assert(E2.accmean() == E2.accuracy());
   assert(E2.accsd() == 0);

   E3.interval(0,false,1,false, 2,1e4L);
   assert(E3.x() == 0.5L);
   assert(E.accuracy() <= 118);
   assert(E3.points().size() == 1);
   E3.update_stats();
   E3.update_accuracies();
   assert(E3.xmin() == 0.5);
   assert(E3.xmax() == 0.5);
   assert(E3.ymin() == E3.y());
   assert(E3.ymax() == E3.y());
   assert(E3.ymean() == E3.y());
   assert(E3.ysd() == 0);
   assert(E3.accmin() == E3.accuracy());
   assert(E3.accmax() == E3.accuracy());
   assert(E3.accmean() == E3.accuracy());
   assert(E3.accsd() == 0);

   E4.interval(-1,true,0.5,true, 1,1e4L);
   assert(E4.x() == 0.5L);
   assert(E.accuracy() <= 118);
   assert(E4.points().size() == 2);
   E4.update_stats();
   E4.update_accuracies();
   assert(E4.xmin() == -1);
   assert(E4.xmax() == 0.5);
   assert(accuracy(0.5, E4.ymin()) <= 30);
   assert(E4.ymax() == E4.y());
   assert(accuracy(1.25, E4.ymean()) <= 52);
   assert(accuracy(0.75, E4.ysd()) <= 133);
   assert(E4.accmin() == 30);
   assert(E4.accmax() == 118);
   assert(E4.accmean() == midpoint(30,118));
   assert(E4.accsd() == 44);

   E5.interval(-0.1L,true,0.5,true, 2,1e4L);
   assert(E5.x() == 0.5L);
   assert(E.accuracy() <= 54);
   assert(E5.points().size() == 3);
   E5.update_stats();
   E5.update_accuracies();
   assert(E5.xmin() == -0.1L);
   assert(E5.xmax() == 0.5);
   assert(accuracy(10.0L/11, E5.ymin()) <= 32);
   assert(E5.ymax() == E5.y());
   assert(accuracy(61.0L/44, E5.ymean()) <= 37);
   assert(accuracy(sqrt(201.0L)/11/cb(Sqr2), E5.ysd()) <= 60);
   assert(E5.accmin() == 31);
   assert(E5.accmax() == 54);
   assert(E5.accmean() == 127.0L/3);
   assert(E5.accsd() == sqrt(794.0L)/3);

   E6.interval(-1,true,0.1L,true, 2,1e4L);
   assert(E6.x() == -1);
   assert(E.accuracy() <= 4);
   assert(E6.points().size() == 3);
   E6.update_stats();
   E6.update_accuracies();
   assert(E6.xmin() == -1);
   assert(E6.xmax() == 0.1L);
   assert(accuracy(0.5L, E6.ymin()) <= 4);
   assert(accuracy(10.0L/9, E6.ymax()) <= 52);
   assert(accuracy(1201.0L/1566, E6.ymean()) <= 41);
   assert(accuracy(11*sqrt(661.0L)/783/Sqr2, E6.ysd()) <= 92);
   assert(E6.accmin() == 4);
   assert(E6.accmax() == 52);
   assert(E6.accmean() == 77.0L/3);
   assert(E6.accsd() == sqrt(3554.0L)/3);

   E7.interval(-1,true,0.5L,true, 3,1e4L);
   assert(E7.x() == 0.5);
   assert(E7.accuracy() <= 118);
   assert(E7.points().size() == 4);
   E7.update_stats();
   E7.update_accuracies();
   assert(E7.xmin() == -1);
   assert(E7.xmax() == 0.5L);
   assert(accuracy(0.5L, E7.ymin()) <= 25);
   assert(accuracy(2, E7.ymax()) <= 118);
   assert(accuracy(25.0L/24, E7.ymean()) <= 25);
   assert(accuracy(sqrt(65.0L)/8/sqrt(3.0L), E7.ysd()) <= 104);
   assert(E7.accmin() == 0);
   assert(E7.accmax() == 118);
   assert(E7.accmean() == 149.0L/4);
   assert(E7.accsd() == sqrt(36139.0L)/4);

   E.interval(0,true,1,false, 2,1e4L);
   assert(E.x() == 0.5L);
   assert(E.accuracy() <= 118);
   assert(E.points().size() == 2);
   assert((E.points().front() == XY8_r1::point_t{0,1}));
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

   E.interval(-1.5,true,0.5,true, 2,1e4L);
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

   E.interval(-1.5,false,0,false, 3,1e4L);
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
   XY8_r1 E(1,0,F,sol);

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
   XY6_r1 E(0,1,F,sol);

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

  {XY8_e s(0,1,[](float80, float80 y){return y;}, [](float80 x){return FP::exp(x);});
   s.interval(0,true, 2,true, 10, 1e4);
  }

  {const auto F = [](float80, float80 y){return y*y;};
   float80 c = 1;
   const auto sol = [&c](float80 x){return c / (1-c*x);};
   XY8_r1 s(0,1,F,sol);
   s.interval(0,true,1,false, 1,1e4L);
  }
}
