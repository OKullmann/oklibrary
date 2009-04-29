/*!
  \file Satisfiability/Solvers/March/equivalence.h
  \brief Updated version to move inline functions to headers.
*/
#include <assert.h>
#include "common.h"

int equivalence_reasoning();

void init_equivalence();
void dispose_preprocessor_eq();

int find_and_remove_tautologies();
void fixDependedEquivalences();
int find_and_propagate_bieq();

int substitude_equivalences( );
void substitude_equivalence( int clause, int index );
void substitude_ceq( int ceqsource, int ceqtarget );

void shorten_equivalence( );

void restoreVeq();

int find_equivalence( int clause );

int propagate_bieq();

void replace_bieq( int lit1, int lit2, int ***__Vc, int ***__VcLUT );

int root_reduce_equivalnces( int nrval, int literal );

int check_non_tautological_equivalences();

int dependantsExists();

inline void fixEq( int varnr, int pos, int value )
{
	int i, last = --Veq[ varnr ][ 0 ];
	int ceqidx = Veq   [ varnr ][ pos ];
        int ceqloc = VeqLUT[ varnr ][ pos ];
        int lit = Ceq[ ceqidx ][ --CeqSizes[ ceqidx ] ];

	assert( lit <= nrofvars );
	assert( lit > 0 );

        for( i = 1; Veq[ lit ][ i ] != ceqidx; i++ );
        VeqLUT[ lit ][ i ] = ceqloc;

        Ceq[ ceqidx ][ ceqloc ] = lit;
        Ceq[ ceqidx ][ CeqSizes[ ceqidx ] ] = varnr;

        CeqValues[ ceqidx ] *= value;

	Veq[ varnr ][ pos  ] = Veq[ varnr ][ last ];
	Veq[ varnr ][ last ] = ceqidx;

	VeqLUT[ varnr ][ pos  ] = VeqLUT[ varnr ][ last ];
	VeqLUT[ varnr ][ last ] = CeqSizes[ ceqidx ]; 
}


void add_binary_equivalence();

void printCeq();
void printNrofEq();
void check_integrety();
