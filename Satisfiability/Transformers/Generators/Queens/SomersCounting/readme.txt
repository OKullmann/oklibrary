Jeff Somers

Copyright (c) 2002

jsomers@alumni.williams.edu
or
allagash98@yahoo.com

April, 2002

Program:  nq

Program to find number of solutions to the N queens problem.
This program assumes a twos complement architecture.

For example, you can arrange 4 queens on 4 x 4 chess so that
none of the queens can attack each other:

Two solutions:
   _ Q _ _        _ _ Q _
   _ _ _ Q        Q _ _ _
   Q _ _ _        _ _ _ Q
   _ _ Q _    and _ Q _ _

Note that these are separate solutions, even though they
are mirror images of each other.

Likewise, a 8 x 8 chess board has 92 solutions to the 8 queens
problem.

Command Line Usage:

        nq N

     where N is the size of the N x N board.  For example,
     nq 4 will find the 4 queen solution for the 4 x 4 chess
     board.

By default, this program will only print the number of solutions,
not board arrangements which are the solutions.  To print the
boards, uncomment the call to printtable in the Nqueen function.
Note that printing the board arrangements slows down the program
quite a bit, unless you pipe the output to a text file:

nq 10 > output.txt


The number of solutions for the N queens problems are known for
boards up to 23 x 23.  With this program, I've calculated the
results for boards up to 21 x 21, and that took over a week on
an 800 MHz PC.  The algorithm is approximated O(n!) (i.e. slow),
and calculating the results for a 22 x 22 board will take about 8.5
times the amount of time for the 21 x 21 board, or over 8 1/2 weeks.
Even with a 10 GHz machine, calculating the results for a 23 x 23
board would take over a month.  Of course, setting up a cluster of
machines (or a distributed client) would do the work in less time.

(from Sloane's On-Line Encyclopedia of Integer Sequences,
 Sequence A000170
 http://www.research.att.com/cgi-bin/access.cgi/as/njas/sequences/eisA.cgi?Anum=000170
 )

 Board Size:       Number of Solutions to          Time to calculate
 (length of one        N queens problem:              on 800MHz PC
  side of N x N                                    (Hours:Mins:Secs)
  chessboard)

   1                                  1                    n/a
   2                                  0                   < 0 seconds
   3                                  0                   < 0 seconds
   4                                  2                   < 0 seconds
   5                                 10                   < 0 seconds
   6                                  4                   < 0 seconds
   7                                 40                   < 0 seconds
   8                                 92                   < 0 seconds
   9                                352                   < 0 seconds
  10                                724                   < 0 seconds
  11                               2680                   < 0 seconds
  12                              14200                   < 0 seconds
  13                              73712                   < 0 seconds
  14                             365596                  00:00:01
  15                            2279184                  00:00:04
  16                           14772512                  00:00:23
  17                           95815104                  00:02:38
  18                          666090624                  00:19:26
  19                         4968057848                  02:31:24
  20                        39029188884                  20:35:06
  21                       314666222712                 174:53:45
  22                      2691008701644                     ?
  23                     24233937684440                     ?
  24                                  ?                     ?
