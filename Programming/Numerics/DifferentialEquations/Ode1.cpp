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
        "0.3.1",
        "22.2.2021",
        __FILE__,
        "Oliver Kullmann",
        "https://github.com/OKullmann/oklibrary/blob/master/Programming/Numerics/DifferentialEquations/Ode1.cpp",
        "GPL v3"};

  namespace FP = FloatingPoint;
  using namespace Ode;

  typedef FP::float80 Float_t; // BUG gcc 10.1.0 : can't use "float_t"
  typedef RK41d<Float_t> RK_t;

  int window1, window2;
  RK_t* rk;

#include "Ode1.fun"

  void display() noexcept {
    glutSetWindow(window1);
    Plot::Draw D1(rk->points(), rk->xmin(),rk->xmax(), rk->ymin(),rk->ymax());
    D1.flush();

    glutSetWindow(window2);
    D1.flush();
    D1.yzero();
    Plot::Draw D2(rk->accuracies(), rk->xmin(),rk->xmax(), rk->accmin(),rk->accmax());
    D2.cred=0; D2.cgreen=0; D2.cblue=1; D2.graph();
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
  if (argc != 5 and argc != 6) return 1;

  const FP::float80 xmin = FP::to_float80(argv[1]),
    xmax = FP::to_float80(argv[2]);
  const FP::UInt_t N = FP::toUInt(argv[3]),
    ssi = FP::toUInt(argv[4]);
  const bool with_iN = argc == 6;
  const FP::UInt_t iN = with_iN ? FP::toUInt(argv[5]) : 0;

  rk = new RK_t(x0,y0h,F,sol); // GCC BUG 10.1.0 "y0 is ambiguous"
  if (with_iN) rk->interval(xmin,true, xmax,true, N, ssi, iN);
  else rk->interval(xmin,true, xmax,true, N, ssi);
  rk->update_stats(); rk->update_accuracies();

  FP::fullprec_floatg<Float_t>(std::cout);
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
