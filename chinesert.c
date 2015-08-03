// Copyright 2008 Philip Greggory Lee.
// Contact me at: rocketman768@gmail.com

// This file is part of KregularKompute.
//
// KregularKompute is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// KregularKompute is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with KregularKompute.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <gmp.h>
#include "algorithms.h"

/* Computes the solution to x = a[i] (mod m[i]) using */
/* Chinese Remainder Theorem.                         */
/* NOTE: you need to make sure the m[i] are all       */
/* relatively prime. Else, this won't work.           */
/* Stores the result in x.                            */
/* The first time the function runs, the algorithm is */
/* set to the particular array m[] that you gave it.  */
/* If you want to reset it, call crt_reset.           */
void crt( mpz_t x, unsigned long int *a, unsigned long int *m, unsigned long int elts )
{
   unsigned long int i;

   mpz_set_ui( x, 0 );

   // Only calculate M once and then leave it alone
   // unless someone calls crt_reset().
   if( !_M_calculated )
   {
      #ifdef CRT_STANDALONE
      // Initialize everything.
      mpz_init( _M );
      mpz_init( _Mi );
      mpz_init( _ztemp );
      mpz_init( _yi );
      #endif

      mpz_set_ui( _M, 1 );

      // Calculate _M to be product of all m[i]
      for( i=0; i < elts; i++ )
         mpz_mul_ui( _M, _M, m[i] );
         
      _M_calculated = 1;
   }
   
   for( i=0; i < elts; i++ )
   {
      mpz_divexact_ui( _Mi, _M, m[i] );
      mpz_set_ui( _ztemp, m[i] );
      // _yi = _Mi^-1 (mod _M)
      mpz_invert( _yi, _Mi, _ztemp );

      // x += a[i]*_Mi*_yi (mod _M)
      mpz_mul_ui( _Mi, _Mi, a[i] );
      mpz_mul( _Mi, _Mi, _yi );
      mpz_mod( _Mi, _Mi, _M );
      mpz_add( x, x, _Mi );

   }

   mpz_mod( x, x, _M );

   return;
}


void crt_reset()
{
   _M_calculated = 0;
}

