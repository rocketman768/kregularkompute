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
#include <math.h>
#include "algorithms.h"

/* Computes g's in the "k"-regular partition function from 1 to "high" */
/* returns NULL on failure */
void gcompute( unsigned long int *ret, unsigned long int k, unsigned long int high, unsigned long int modulus )
{
   unsigned long int m, e;
   double sqrt_m;
   unsigned long long int crap;

   if( ret == NULL )
   {
      fprintf(stderr, "\n\tgcompute(): Asshole. Can't even give me an allocated array?...sheesh.\n");
      die();
   }

   ret[0] = 0;

   for( m=1; m <= high; m++ )
   {
      ret[m] = 0;
      sqrt_m = sqrt(m);
      
      for( e=1; e <= sqrt_m; e++ )
      {
         if( m % e == 0 )
         {
            if( e % k != 0 )
            {
               crap = ret[m]+e;
               ret[m] = crap % modulus;
            }
            if( (m/e) % k != 0 && (m/e) != e )
            {
               crap = ret[m] + (m/e);
               ret[m] = crap % modulus;
            }
         }
      }
      
   }
       
   return;
}      

