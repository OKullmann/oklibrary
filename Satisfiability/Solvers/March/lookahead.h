/* MARCH II! Satisfiability Solver

   Copyright (C) 2001 M. Dufour, M. Heule, J. van Zwieten
   [m.dufour@student.tudelft.nl, marijn@heule.nl, zwieten@ch.tudelft.nl]

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef __LOOKAHEAD_H__
#define __LOOKAHEAD_H__

void init_lookahead  ();
void destroyLookahead();

int lookahead();

       int treelookvar			( const int nrval );
       int look_IFIUP			( const int nrval );
       int look_fix_forced_literal	( const int nrval );

inline int look_fix_big_clauses         ( const int nrval );
inline int look_fix_binary_implications	( const int nrval );
inline int look_fix_equivalences	( const int nrval );
inline int look_fix_ternary_implications( const int nrval );
inline int add_constraint_resolvent	( const int nrval );
inline void add_resolvents		(		  );

inline void restore_big_clauses ( int *head_stackp, int *tail_stackp );
       void evaluate_big_clauses( int *head_stackp, int *tail_stackp );

inline void look_backtrack();

void lookahead_subsume( const int nrval );

void get_forced_literals( int **_forced_literal_array, int *_forced_literals );
int  get_signedBranchVariable( );

int check_all_equal( int clsidx );

void ComputeDiffWeights( );

void cleanLookahead();
void cleanFormula();

void reset_lookahead_resstack_head( );
int    get_lookahead_resstack_head( );

inline int ternarylook_fix_direct_implications ( const int parent, const int reference );
inline int ternarylook_fix_ternary_implications( const int nrval );
void   ternarylook_detect_constraint_resolvents( const int lit1, const int lit2 );
int    ternarylook_add_constraint_resolvents   ( const int lit1, const int lit2 );

#endif
