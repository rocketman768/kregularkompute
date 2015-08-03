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

#include <math.h>
#include "algorithms.h"

/* Returns 1 if "n" is prime or 0 otherwise. */

int isprime( unsigned long int n )
{
   unsigned long int sqrt_n, i, residue;

   sqrt_n = (unsigned long int)sqrt( (double)n ) + 1;

   // Get rid of the two cases not automatically
   // taken care of by the rest of the algorithm.
   // I.e. 2 or 3 divides the number.
   if( (n&0x1) == 0 || n % 3 == 0 )
      return 0;

   residue = 5;
   for( i=5; i <= sqrt_n; )
   {
      // Take advantage of the fact that all
      // primes past 2 and 3 are +-1 (mod 6)
      if( n % i == 0 )
         return 0;

      if( residue == 1 )
         i += 4;
      else      // This means residue equals 5.
         i +=2;

      residue = i % 6;
   }

   return 1;
}

/* Gets the next prime above "n". */

unsigned long int nextprime( unsigned long int n )
{
   unsigned long int next, residue;

   next = n+1;
   residue = next % 6;

   while( residue != 1 && residue != 5 )
   {
      next++;
      residue = next % 6;
   }

   while( !isprime(next) )
   {
      residue = next % 6;
      if( residue == 1 )
         next += 4;
      else
         next += 2;
   }

   return next;
}

