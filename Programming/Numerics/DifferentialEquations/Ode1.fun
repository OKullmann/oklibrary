const Float_t c = 10;

const RK_t::F_t F = [](const Float_t x, const Float_t y)
  {return std::fma(std::cos(x), y + x*x, - 2*x);};

const RK_t::f_t sol = [](const Float_t x)
  {return c * std::exp(std::sin(x)) - x*x;};

const Float_t x0 = 0, y0h = sol(x0);
