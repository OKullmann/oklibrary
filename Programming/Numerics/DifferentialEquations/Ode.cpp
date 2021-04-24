// Oliver Kullmann, 22.4.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  > Ode min_x max_x num_int num_subint [iN] [option=+g|-g]

  Compile with
    idir=PATH-TO-DIRECTORY-WITH-FUNi-FILES/
  to use
    PATH/Ode.fun0/1/2/3
  instead of the Ode.fun0/1/2/3 placed in this directory.

TODOS:

1. Generalise to higher dimensions
   - DONE
     For the dimension of the function to be modelled, n is used.
   - DONE Defined in Ode.fun1.
   - Is it possible to make the projections of the numeric solution available
     as y1, ..., yn (predefined function-constants, just the projections)?


*/

#include <iostream>
#include <vector>
#include <array>
#include <utility>
#include <string>

#include <cmath>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glut.h>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Visualisation/Graphics/Plot.hpp>

#include "Ode.hpp"
#include "Stepper.hpp"
#include "Windows.hpp"

#ifndef IFUN0
# define IFUN0 Ode.fun0
#endif
#ifndef IFUN1
# define IFUN1 Ode.fun1
#endif

#ifndef IFUN2
# define IFUN2 Ode.fun2
#endif

#ifndef IFUN3
# define IFUN3 Ode.fun3
#endif


#include STR(IFUN0)

namespace Oden {

  enum class GraphO {with=0, without=1}; // MUST correspond to Registration
  constexpr int GraphOsize = 2;
  constexpr GraphO default_grapho = GraphO::with;
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Oden::GraphO> {
    static constexpr int size = Oden::GraphOsize;
    static constexpr std::array<const char*, size> string
    {"+g", "-g"};
  };
}
namespace Oden {
  std::ostream& operator <<(std::ostream& out, const GraphO g) {
    switch (g) {
    case GraphO::with : return out << "with-graphs";
    default : return out << "without-graphs";}
  }

  enum class Error {
    option = 1,

  };

}

namespace {

  const Environment::ProgramInfo proginfo{
        "0.2.1",
        "24.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferentialEquations/Ode.cpp",
        "GPL v3"};

  namespace FP = FloatingPoint;
  using namespace Ode;
  using namespace Oden;
  using namespace Stepper;
  using namespace Windows;

// Defines Float_t, RK_t, num_windows :
#include STR(IFUN1)
typedef XY_t::F_t F_t;
typedef XY_t::f_t f_t;
typedef XY_t::x_t x_t;
typedef XY_t::y_t y_t;
typedef std::function<Float_t(x_t,y_t)> Fp_t;

  constexpr Float_t xmin_d = -10, xmax_d = 10;
  constexpr FP::UInt_t N_d = 1000, Ns_d = 1000, iN_d = XY_t::default_N;

  const std::string error = "ERROR[" + proginfo.prg + "]: ";

  bool show_usage(const int argc, const char* const argv[]) {
    if (not Environment::help_header(std::cout, argc, argv, proginfo))
      return false;
    std::cout <<
      "> "<<proginfo.prg<< " [xmin=" << xmin_d << "] [xmax=" << xmax_d <<
      "] [N=" << N_d << "] [Ns=" << Ns_d << "] [iN=" << iN_d << "] "
      "[options]\n\n"
    " options : " << Environment::WRP<GraphO>{} << "\n\n"
    " computes the mesh, plots it if enabled, and waits for further window-input:\n\n"
    "  - The default values are always created by \"\".\n"
    "  - Trailing arguments can be left out, then using their default-values.\n"
 ;
    return true;
  }


  GraphO to_GraphO(const std::string_view arg) noexcept {
    if (arg.empty()) return GraphO::with;
    const std::optional<GraphO> o = Environment::read<GraphO>(arg);
    if (not o) {
      std::cerr << error << "Invalid option-parameter: \"" << arg << "\".\n";
      std::exit(int(Error::option));
    }
    return o.value();
  }


  XY_t* rk;

  std::array<int, num_windows> list_windows;

  struct EF_t {
    const Fp_t f;
    const std::string name;
    const bool y0; // show y=0 axis?
    const bool rp = false; // restricted precision ?
    const bool sn = false; // scientific notation?

    EF_t(Fp_t f, const std::string n, bool y0, const bool rp = false, const bool sn = false)
      noexcept : f(f), name(n), y0(y0), rp(rp), sn(sn) {}
    EF_t(Fp_t f, const std::string n) noexcept : f(f), name(n), y0(false) {}

    Float_t operator()(const x_t x, const y_t& y) const noexcept {
      return f(x,y);
    }
  };
  typedef std::vector<EF_t> list_functions_t;
  typedef std::array<list_functions_t, num_windows> list_plots_t;

  list_plots_t plots;

  const F_t y = [](Float_t, y_t y){return y;};
  const F_t acc = [](Float_t x, y_t y){
    return FP::accuracyv(rk->sol(x), y, FP::PrecZ::eps);};

  typedef Plot::UnitCubes<Float_t>::points_vt points_vt;
  typedef std::vector<points_vt> list_points_t;
  typedef std::array<list_points_t, num_windows> list_numplots_t;

  list_numplots_t numplots;
  void produce_numplots() {
    const auto& pv = rk->points();
    const auto size = pv.size();
    for (unsigned i = 0; i < num_windows; ++i) {
      for (const EF_t& F : plots[i]) {
        points_vt p; p.reserve(size);
        for (const auto [x,y] : pv) p.push_back({x, F(x,y)});
        numplots[i].push_back(std::move(p));
      }
    }
  }

  void output_statistics(std::ostream& out) {
    for (unsigned i = 0; i < num_windows; ++i) {
      out << "Window: " << i << "\n\n";
      for (std::size_t j = 0; j < plots[i].size(); ++j) {
        const auto& fun = plots[i][j];
        const auto& P = numplots[i][j];
        typedef XY_t::stats_t stats_t;
        const stats_t s(P);
        const stats_t::Format fo(fun.name.c_str(), fun.rp, fun.sn);
        s.out(out, fo);
        out << "\n";
      }
    }
  }


  void display() noexcept {
    for (unsigned i = 0; i < num_windows; ++i) {
      glutSetWindow(list_windows[i]);
      for (unsigned j = 0; j < numplots[i].size(); ++j) {
        Plot::Draw D(numplots[i][j]);
        D.plot_colour(Plot::first_colours[j]);
        if (j == 0) D.new_plot(plots[i][j].y0);
        else D.graph(plots[i][j].y0);
      }
    }
  }

  void menu_handler(const int v) noexcept {
    if (v == 0) glutDisplayFunc(display);
    else if (v == 1) {
      for (unsigned i = 0; i < num_windows; ++i)
        glutDestroyWindow(list_windows[i]);
      std::exit(0);
    }
    glutPostRedisplay();
  }

  void create_menu() noexcept {
    [[maybe_unused]] const auto menu = glutCreateMenu(menu_handler);
    glutAddMenuEntry("Run", 0);
    glutAddMenuEntry("Exit", 1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
  }


  void init_glut() noexcept {
    {int argc = 1; char* argv[1] = {(char*) "Oden"};
     glutInit(&argc, argv);}
    glutInitDisplayMode(GLUT_SINGLE);
  }

}

int main(const int argc, const char* const argv[]) {
  if (Environment::version_output(std::cout, proginfo, argc, argv)) return 0;
  if (show_usage(argc, argv)) return 0;

  Environment::Index index;

  using std::string_view; using FP::to_float80; using FP::toUInt;
  const FP::float80 xmin = argc <= index ? xmin_d :
    string_view(argv[index++]).empty() ? xmin_d : to_float80(argv[index-1]);
  const FP::float80 xmax = argc <= index ? xmax_d :
    string_view(argv[index++]).empty() ? xmax_d : to_float80(argv[index-1]);

  const FP::UInt_t N = argc <= index ? N_d :
    string_view(argv[index++]).empty() ? N_d : toUInt(argv[index-1]);
  const FP::UInt_t ssi = argc <= index ? Ns_d :
    string_view(argv[index++]).empty() ? Ns_d : toUInt(argv[index-1]);

  const FP::UInt_t iN = argc <= index ? iN_d :
    string_view(argv[index++]).empty() ? iN_d : toUInt(argv[index-1]);
  const GraphO go = argc <= index ? GraphO{} :
    string_view(argv[index++]).empty() ? GraphO{} : to_GraphO(argv[index-1]);

  index.deactivate();


#include STR(IFUN2)

  rk = new XY_t(x0,y0h,F,sol); // GCC BUG 10.1.0 "y0 is ambiguous"
  assert(rk->size == n);
  rk->interval(xmin,true, xmax,true, N, ssi, iN);

  std::cout << *rk;

#include STR(IFUN3)
  produce_numplots();

  output_statistics(std::cout);
  std::cout.flush();

  if (go == GraphO::without) return 0;

  init_glut();
  init_windows(num_windows, list_windows);

  create_menu();
  glewInit();
  display();
  glutMainLoop();

}
