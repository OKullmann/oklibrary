typedef FP::float80 Float_t; // BUG gcc 10.1.0 : can't use "float_t"
typedef RK41d<Float_t> RK_t;
typedef RK_t::F_t F_t;
typedef RK_t::f_t f_t;


const Float_t c = 10;

// F(t, y) = cos(x) * (y + x^2) - 2x
const F_t F = [](const Float_t x, const Float_t y)
  {return std::fma(std::cos(x), y + x*x, - 2*x);};

// y(t) = exp(sin(x)) - x^2
const f_t sol = [](const Float_t x)
  {return c * std::exp(std::sin(x)) - x*x;};

const Float_t x0 = 0, y0h = sol(x0);
