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
#include <stdlib.h>
#include "algorithms.h"

/* Computes number^(-1) (mod modulus) using Euler's method. */
/* Returns the answer if an answer exists, or 0 otherwise.  */

unsigned long int inverse( unsigned long int number, unsigned long int modulus )
{
   unsigned long int i, ret, i_mod3, im1_mod3, im2_mod3;

   #ifdef INV_STANDALONE
      _remainder = malloc( 3 * sizeof(unsigned long) );
      _quotient = malloc( 3 * sizeof(unsigned long) );
      _auxiliary = malloc( 3 * sizeof(long int) );
   #endif
   
   _remainder[0] = modulus;
   _remainder[1] = number;
   _auxiliary[0] = 0;
   _auxiliary[1] = 1;

   for( i=2; _remainder[(i-1) % 3] > 1; i++ )
   {
      i_mod3 = i%3; im1_mod3 = (i-1) % 3; im2_mod3 = (i-2) % 3;
      _remainder[i_mod3] = _remainder[im2_mod3] %  _remainder[im1_mod3];
      _quotient[i_mod3] = _remainder[im2_mod3] / _remainder[im1_mod3];
      _auxiliary[i_mod3] = -1*_quotient[i_mod3]*_auxiliary[im1_mod3]+_auxiliary[im2_mod3];
   }

   if( _auxiliary[(i-1)%3] < 0 )
      ret = modulus + _auxiliary[(i-1)%3];
   else
      ret = _auxiliary[(i-1)%3];

   #ifdef INV_NOCHECK
      // Make sure we got the right answer.
      if( (unsigned long long)ret*(unsigned long long)number % (unsigned long long)modulus != 1 )
         ret = 0;
   #endif
   
   return ret;
}

