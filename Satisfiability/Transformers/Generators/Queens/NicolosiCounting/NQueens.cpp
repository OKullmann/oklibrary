// Oliver Kullmann 25.5.2018 (Swansea)
/*
  Copyright Alessandro Nicolosi 2016, 2017
  Copyright Oliver Kullmann 2018

MIT License

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

  Started as copy from
  https://github.com/alenic/nqueens/commits/master
  ID f50264291093fe8529d143995ae9784f097d7ebf.
*/

#include <cstdint>
#include <iostream>
#include <string>

namespace {

typedef std::uint_fast64_t count_t;
typedef std::uint32_t queen_t;

count_t count = 0;

inline void backtracking(const queen_t mask, const queen_t pos, const queen_t r, const queen_t d1, const queen_t d2) noexcept {
  for (queen_t c = ~pos & mask; c;) {
    const queen_t i = -c & c;
    const queen_t rpi = r + i;
    if (rpi == mask) {++count; return;} else {
      const queen_t newd1 = (d1+i) >> 1, newd2 = (d2+i) << 1;
      backtracking(mask, rpi|newd1|newd2, rpi, newd1, newd2);
      c -= i;
    }
  }
}

}

int main(const int argc, const char* const argv[]) {
  if (argc != 2) { std::cout << "Usage[qcount]: N\n"; return 0; }
  const unsigned long arg1 = std::stoul(argv[1]);
  if (arg1 == 0) { std::cout << 1 << "\n"; return 0; }
  if (arg1 > 32) { std::cerr << " N <= 32 required.\n"; return 1; }
  const queen_t N = arg1;
  const queen_t mask = (1 << N) - 1;
  if (N % 2 == 0) {
    backtracking(mask, (1 << (N / 2)) - 1, 0, 0, 0);
    std::cout << 2*count;
  } else {
    backtracking(mask, (1 << ((N / 2) + 1)) - 1, 0, 0, 0);
    const count_t count1 = count;
    count = 0;
    backtracking(mask, ~(1 << (N / 2)), 0, 0, 0);
    std::cout << 2*count1 + count;
  }
}
