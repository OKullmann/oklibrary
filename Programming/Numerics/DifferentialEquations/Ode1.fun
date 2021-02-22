const FP::float80 c = 10;

const RK41d_80::F_t F = [](const FP::float80 x, const FP::float80 y)
  {return FP::fma(FP::cos(x), y + x*x, - 2*x);};

const RK41d_80::f_t sol = [](const FP::float80 x)
  {return c * FP::exp(FP::sin(x)) - x*x;};

const FP::float80 x0 = 0, y0h = sol(x0);
