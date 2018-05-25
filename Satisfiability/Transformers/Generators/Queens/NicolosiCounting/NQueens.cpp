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

#include <stdio.h>

int mask;
unsigned long long sol = 0;

void nqueens_backtracking(int pos, int r, int d1, int d2) {
  int c = ~pos & mask;
  while(c) {
    int i = -c & c, newd1, newd2;
      if (r+i == mask) {
        ++sol;
        return;
      }
      newd1 = (d1 + i) >> 1,
      newd2 = (d2 + i) << 1;
      nqueens_backtracking(((r+i) | newd1 | newd2), r+i ,newd1, newd2);
      c -= i;
   }
}

int main(int argc, char **argv)
{
  int N;

  printf("Insert the dimension of chekboard: ");
  scanf("%d", &N);

  if(N < 0 || N > 32) {
    printf("Error: N must be an integer between 0 and 32\n");
    return -1;
  }

  mask = (1 << N) - 1;

  if (N % 2 == 0)
  {
    nqueens_backtracking((1 << (N / 2)) - 1, 0, 0, 0);
    printf("Number of all solutions: %llu\n", 2*sol);
  } else {
    unsigned long long sol1;
    nqueens_backtracking((1 << ((N / 2) + 1)) - 1, 0, 0, 0);
    sol1 = sol;
    sol = 0;
    nqueens_backtracking(~(1 << (N / 2)), 0, 0, 0);
    printf("Number of all solutions: %llu\n", 2*sol1 + sol);
  }

  return 0;
}
