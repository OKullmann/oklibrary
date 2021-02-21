// Oliver Kullmann, 6.2.2021 (Swansea)
/* Copyright 2021 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*

*/

#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <Numerics/FloatingPoint.hpp>
#include <ProgramOptions/Environment.hpp>
#include <Visualisation/Graphics/Plot.hpp>

#include "Ode.hpp"

namespace {

  const Environment::ProgramInfo proginfo{
        "0.1.0",
        "20.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferentialEquations/Ode1.cpp",
        "GPL v3"};

  namespace FP = FloatingPoint;
  using namespace Ode;

  int window1, window2;
  RK41d_80* rk;

  const RK41d_80::F_t F = [](const FP::float80 x, const FP::float80 y)
  {return FP::fma(FP::cos(x), y + x*x, - 2*x);};
  const RK41d_80::f_t sol = [](const FP::float80 x)
  {return 10 * FP::exp(FP::sin(x)) - x*x;};
  const FP::float80 x0 = 0, y0h = sol(x0);


  void display() noexcept {
    glutSetWindow(window1);
    Plot::Draw D1(rk->points(), rk->xmin(),rk->xmax(), rk->ymin(),rk->ymax());
    D1.flush();
    glutSetWindow(window2);
    Plot::Draw D2(rk->accuracies(), rk->xmin(),rk->xmax(), rk->accmin(),rk->accmax());
    D2.flush();
  }

  void menu_handler(const int v) noexcept {
    if (v == 0) glutDisplayFunc(display);
    else if (v == 1) {
      glutDestroyWindow(window1); glutDestroyWindow(window2);
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

}

int main(int argc, char** const argv) {
  glutInitWindowSize(800, 800);
  glutInitWindowPosition(100, 2000);
  glutInit(&argc, argv);

  if (Environment::version_output(std::cout, proginfo, argc, argv))
  return 0;
  if (argc != 3) return 1;

  const FP::float80 xmin = FP::to_float80(argv[1]),
    xmax = FP::to_float80(argv[2]);

  rk = new RK41d_80(x0,y0h,F,sol); // GCC BUG 10.1.0 "y0 is ambiguous"
  rk->interval(xmin,true, xmax,true, 1000, 1000);
  rk->update_stats(); rk->update_accuracies();
  std::cout << *rk; std::cout.flush();

  glutInitDisplayMode(GLUT_SINGLE);
  window1 = glutCreateWindow("Solution");
  glutInitWindowPosition(1100, 2000);
  window2 = glutCreateWindow("Accuracy");
  glutSetWindow(window1);

  create_menu();
  glewInit();
  glutMainLoop();

}
