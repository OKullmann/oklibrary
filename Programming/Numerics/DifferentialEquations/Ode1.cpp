// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

  USAGE:

  > Ode1 min_x max_x num_int num_subint [iN] [option=+g|-g]

  Compile with
    idir=PATH-TO-DIRECTORY-WITH-FUNi-FILES/
  to use
    PATH/Ode1.fun0/1/2/3
  instead of the Ode1.fun0/1/2/3 placed in this directory.

TODOS:

0. For the current example: check accuracy of solution
   - We have for sol(x) = c*exp(sin(x)) - x*x:
       sol(x) = fma(c, expm1(sin(x)), c-x*x);
     perhaps this is more precise?
   - Just using the above for sol doesn't seem to change much (only tiny
     differences). But one should do a precise comparison between the
     two solution-functions.
   - One should also compare with computations with higher precision.

2. Enable plotting of arbitrary functions (many of them)
   - DONE
     In Ode1.fun one specifies the functions which go into window 1/2 (or even
     more windows).
     - DONE We use one universal x-interval.
     - For the dimension of the function to be modelled, n is used.
     - DONE
       One likely needs another auxiliary file: Ode.def for definitions of
      - n
      - Float_t
      - solver (default is RK4)
     - The projections of the numeric solution are available as
       y1, ..., yn (predefined function-constants, just the projections).
       And y is the whole vector-function.
     - Accuracy is acc (predefined),
     - Any other function can be defined using x and the vector y as
       arguments -- there is a type vec available, with components
       y[0], ..., y[n-1]. So all these functions have the same inputs as
       the master-function F: F(x,y).
     - Perhaps best to have F_1(x,y), ..., F_n(x,y), always.
     - DONE
       Best to include Ode.fun after the solution has been computed: then
       the real function-objects can be directly constructed.
       But for that one needed actually three auxiliary files: one for the
       basic definitions, one for the equations F_i(x,y), one for the plotting.
       Perhaps they are named Ode.fun(1,2,3).
       acc is then available as a vector-function acc(y,y'), with y the precise
       vector, and y' the approximation to be measured.
     - DONE Per window one specifies the list of functions to be plotted.
   - DONE
     These functions yield "plots", the vector of points and accompanying
     parameters.
   - One parameter is the colour, and there should be defaults for automatic
     handling. Perhaps a fixed list of (named) colours.

     In an additional window the colours used are clearly plotted.
     Perhaps in the order of the functions one horizontal bar, with one block
     of such bars per plotting-window.
   - DONE
     Another parameter is for the lines y=0 resp. x=0; here we need to handle
     the colour -- we can't just always use white for these lines.
     One possibility is to just use the same colour as for the points.
   - DONE
     For each window one has a list of plots (each in its own scale by default,
     only x-axis is shared).
   - These are the function-plot-windows. There are moving-plot-windows also,
     given as follows: There are m 2-dimensional points, given as pairs of
     functions (in the arguments x, y). For each step x_i these points are
     drawn, connected with a line to the corresponding point x_{i-1}; each
     point with its colour. There is a parameter for that window, the total
     time (and the possibility to re-draw the whole sequence).
     Here we need to find a common frame (by default the maximum ranges of
     x- and y-values).
   - Actually, even the x-axis for function-plot-windows does not need to be
     shared -- everything is just rescaled.
   - The statistics-output just applies the standard statistics to all of
     these lists, per window.
   - Then perhaps the numeric output goes into files, one per window.

4. Basic documentation
   - In this file.
   - In a docus-subdirectory.

6. Make also second window resizable
   - Works via re-drawing (currently by "Run").

7. Statistics:
   - Count exact zeros.
   - Count sign-changes.
   - As an option for the menu and commandline: print approximate zeros.
    - In ascending order.
    - For sign changes without zero the average of the two x-values where
      the sign changes, with a leading "~".
    - For single exact zeros the x-value.
    - For intervals of exact zeros the interval of x-values.

8. Options for the menu
   - For both windows.
   - Turning a grid on/off (absolute, for the [-1,+1]-ranges).
   - Lines y=0 and x=0 (on/off): perhaps this is global, for each
     plot its own lines.
   - Turning the central axix-system on/off.

9. Application tests

*/

#include <iostream>
#include <vector>
#include <array>
#include <utility>

#include <cmath>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glut.h>

#include <ProgramOptions/Environment.hpp>
#include <Numerics/FloatingPoint.hpp>
#include <Visualisation/Graphics/Plot.hpp>

#include "Ode.hpp"
#include "Stepper.hpp"

#ifndef IFUN0
# define IFUN0 Ode1.fun0
#endif
#ifndef IFUN1
# define IFUN1 Ode1.fun1
#endif

#ifndef IFUN2
# define IFUN2 Ode1.fun2
#endif

#ifndef IFUN3
# define IFUN3 Ode1.fun3
#endif


#include STR(IFUN0)

namespace Ode1 {

  enum class GraphO {with=0, without=1}; // MUST correspond to Registration
  constexpr int GraphOsize = 2;
  constexpr GraphO default_grapho = GraphO::with;
}
namespace Environment {
  template <>
  struct RegistrationPolicies<Ode1::GraphO> {
    static constexpr int size = Ode1::GraphOsize;
    static constexpr std::array<const char*, size> string
    {"+g", "-g"};
  };
}
namespace Ode1 {
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
        "0.9.2",
        "11.4.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferentialEquations/Ode1.cpp",
        "GPL v3"};

  namespace FP = FloatingPoint;
  using namespace Ode;
  using namespace Ode1;
  using namespace Stepper;

// Defines Float_t, RK_t, num_windows :
#include STR(IFUN1)
typedef XY_t::F_t F_t;
typedef XY_t::f_t f_t;


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
    " computes the mesh, plots it if enabled, and waits for furher window-input:\n\n"
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
    const F_t f;
    const bool y0;
    typedef std::size_t size_t;
    const size_t p;

    EF_t(F_t f, bool y0, const size_t p = 0) noexcept : f(f), y0(y0), p(p) {}
    EF_t(F_t f, const size_t p = 0) noexcept : f(f), y0(false), p(p) {}

    typedef XY_t::x_t x_t;
    template <typename y_t>
    Float_t operator()(const x_t x, const y_t& y) const noexcept {
      return f(x,y[p]);
    }
    Float_t operator()(const x_t x, const Float_t y) const noexcept {
      return f(x,y);
    }
  };
  typedef std::vector<EF_t> list_functions_t;
  typedef std::array<list_functions_t, num_windows> list_plots_t;

  list_plots_t plots;

  const F_t y = [](Float_t, Float_t y){return y;};
  const F_t acc = [](Float_t x, Float_t y){
    return FP::accuracyg<Float_t>(rk->sol(x), y, FP::PrecZ::eps);};

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
      glutDestroyWindow(list_windows[0]); glutDestroyWindow(list_windows[1]);
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
    {int argc = 1; char* argv[1] = {(char*) "Ode1"};
     glutInit(&argc, argv);}
    glutInitDisplayMode(GLUT_SINGLE);
  }

  struct WinPar {
    const int x, y, w, h;
    const std::string n;
    WinPar(int x, int y, int w, int h, std::string n) noexcept :
      x(x), y(y), w(w), h(h), n(n) {}
    int create() const noexcept {
      assert(number == 0);
      glutInitWindowSize(w, h);
      glutInitWindowPosition(x, y);
      return number = glutCreateWindow(n.c_str());
    }
    int index() const noexcept { return number; }
  private :
    mutable int number = 0;
  };

  const WinPar wpar1(100, 2000, 800,800, "Solution");
  const WinPar wpar2(1100,2000, 800,800, "Accuracy");

  const std::array<WinPar, num_windows> list_winpars{wpar1, wpar2};
  void init_windows() noexcept {
    for (unsigned i = 0; i < num_windows; ++i)
      list_windows[i] = list_winpars[i].create();
    glutSetWindow(list_windows[0]);
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
  rk->interval(xmin,true, xmax,true, N, ssi, iN);
  rk->update_stats(); rk->update_accuracies();

  std::cout << *rk; std::cout.flush();

  if (go == GraphO::without) return 0;

  init_glut();
  init_windows();

#include STR(IFUN3)
  produce_numplots();

  create_menu();
  glewInit();
  display();
  glutMainLoop();

}
