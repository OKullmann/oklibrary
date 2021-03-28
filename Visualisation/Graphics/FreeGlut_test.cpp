/*

  > sudo apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev

 Also to be installed: libglew-dev


 TODOS:

 1) Why are the special drawings not permanent?
   - This happens when the first window is active, and one clicks the
     the right-button.
   - While with the second window everything seems fine.

*/

#include <vector>

#include <cstdlib>
#include <cmath>
#include <cstdint>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <Numerics/FloatingPoint.hpp>

#include "Plot.hpp"

namespace {

  static int window, window2;

  struct point {
    GLfloat x, y;
    point() noexcept : x(0), y(0) {}
    point(const GLfloat x, const GLfloat y) noexcept : x(x), y(y) {}
  };



void display() noexcept {
  glClearColor(0.5, 0.5, 0.5, 0);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1, 1, 1);
  glBegin(GL_POLYGON);
  glVertex2f(0.0, 0.0);
  glVertex2f(0.5, 0.0);
  glVertex2f(0.0, 0.5);
  glEnd();

  glColor3f(0, 0, 0);
  glBegin(GL_LINES);
  glVertex2f(-1, 0);
  glVertex2f(1, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex2f(0, -1);
  glVertex2f(0, 1);
  glEnd();

  constexpr double N = 1e6;
  constexpr double begin = -1;
  constexpr double end = 1;
  constexpr double step = (end - begin) / N;

  constexpr double freq = 50, freq2 = 30;
  constexpr auto f = [freq](const double x){return std::sin(freq*x);};
  constexpr auto f2 = [freq2](const double x){return std::cos(freq2*x);};
  constexpr auto f3 = [f,f2](const double x){return (f(x)+f2(x)) / 2;};
  constexpr auto f4 = [](const double x){return 1 / (1 + x*x);};

  glColor3f(1, 1, 1);
  glBegin(GL_LINE_STRIP);
  for (std::uint64_t i = 0; i <= N; ++i) {
    const double x = begin + i * step;
    const double y = f(x);
    glVertex2f(x, y);
  }
  glEnd();

  glColor3f(0, 0, 1);
  glBegin(GL_LINE_STRIP);
  for (std::uint64_t i = 0; i <= N; ++i) {
    const double x = begin + i * step;
    const double y = f2(x);
    glVertex2f(x, y);
  }
  glEnd();

  glColor3f(1, 1, 0);
  glBegin(GL_LINE_STRIP);
  for (std::uint64_t i = 0; i <= N; ++i) {
    const double x = begin + i * step;
    const double y = f3(x);
    glVertex2f(x, y);
  }
  glEnd();

  using std::size_t;
  constexpr size_t S = 10000;
  constexpr double step2 = (end - begin) / S;
  std::vector<point> graph; graph.reserve(S+1);
  for (size_t i = 0; i <= S; ++i) {
    const double x = begin + i * step2;
    const double y = f4(x);
    graph.emplace_back(x,y);
  }
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, graph.size() * sizeof(point), &graph[0], GL_STATIC_DRAW);
  constexpr GLuint attribute_coord2d = 0;
  glEnableVertexAttribArray(attribute_coord2d);
  glVertexAttribPointer(attribute_coord2d, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glDrawArrays(GL_LINE_STRIP, 0, S);
  glDisableVertexAttribArray(attribute_coord2d);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glFlush();
}

void display2() noexcept {
  using FloatingPoint::float80;
  using points_vt = Plot::UnitCubes<float80>::points_vt;
  const float80 a = -10, b = 10;
  const unsigned N = 1000;
  const float80 delta = (b - a) / N;
  const auto f = [](const float80 x){return std::sin(x) * x;};
  points_vt P; P.reserve(N+1);
  for (unsigned i = 0; i <= N; ++i) {
    const float80 x = a + i*delta, y = f(x);
    P.push_back({x,y});
  }
  Plot::Draw D(P);
  D.plot_colour(Plot::yellow);
  D.new_plot(true);
}

  int w;
  void menu_handler(const int v) noexcept {
    if (v == 0) {
      glutDestroyWindow(window); glutDestroyWindow(window2);
      std::exit(0);
    }
    else if (v == 1) {
      assert(w == window or w == window2);
      glutSetWindow(w);
      std::cout << "Show window: " << w << std::endl;
      if (w == window) glutDisplayFunc(display);
      else glutDisplayFunc(display2);
    }
    else if (v == 2) {
      assert(w == window or w == window2);
      if (w == window) w = window2;
      else w = window;
      std::cout << "New window: " << w << std::endl;
      return;
    }
    glutPostRedisplay();
  }
  void submenu_handler(const int v) noexcept {
    //glClearColor(0, 0, 0, 0);
    //glClear(GL_COLOR_BUFFER_BIT);
    assert(w == window or w == window2);
    glutSetWindow(w);
    if (v == 2) {
      glPushMatrix();
      glColor3d(1, 0, 0);
      glutWireSphere(1, 50, 50);
      glPopMatrix();
    }
    else if (v == 3) {
      glPushMatrix();
      glColor3d(0, 1, 0);
      glRotated(65, -1, 0, 0);
      glutWireCone(1, 1, 50, 50);
      glPopMatrix();
    }
    else if (v == 4) {
      glPushMatrix();
      glColor3d(0, 0, 1);
      glutWireTorus(0.3,0.6,100,100);
      glPopMatrix();
    }
    else {
      glPushMatrix();
      glColor3d(1, 0, 1);
      glutSolidTeapot(0.5);
      glPopMatrix();
    }
    glFlush();
  }
  void create_menu() noexcept {
    const auto submenu = glutCreateMenu(submenu_handler);
    glutAddMenuEntry("Sphere", 2);
    glutAddMenuEntry("Cone", 3);
    glutAddMenuEntry("Torus", 4);
    glutAddMenuEntry("Teapot", 5);
    [[maybe_unused]] const auto menu = glutCreateMenu(menu_handler);
    glutAddMenuEntry("Exit", 0);
    glutAddMenuEntry("Run", 1);
    glutAddMenuEntry("Switch", 2);
    glutAddSubMenu("Drawings", submenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
  }

}

int main(int argc, char** const argv) {
  glutInitWindowSize(600, 600);
  glutInitWindowPosition(200, 200);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE);
  window = glutCreateWindow("Simple figures");
  glutInitWindowPosition(800, 800);
  window2 = glutCreateWindow("Second window");

  glutSetWindow(window); w = window;

  std::cout << window << " " << window2 << std::endl;

  create_menu();
  glewInit();
  glutMainLoop();
}
