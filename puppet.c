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
#include <time.h>
#include <gmp.h>

int main(int argc, char *argv[])
{
   unsigned long int n, k, *terms, allTerms, *numDivis, allDivis;
   unsigned long int crap;
   unsigned long int mod;
   double percentage;

   mpz_init(crap);

   if( argc < 3 )
   {
      printf("\nUsage:\n\t%s file modulus\n", argv[0]);
      return 0;
   }

   mod = strtoul( argv[2], NULL, 10 );
   terms = malloc( sizeof(unsigned long int) * mod );
   numDivis = malloc( sizeof(unsigned long int) * mod );
   allTerms = 0;
   allDivis = 0;

   // Initialize arrays to all zeros
   for( n=0; n < mod; n++ )
   {
      terms[n]=0;
      numDivis[n]=0;
   }

   for( n=1; !feof(fp); n++ )
   {
      fscanf( stdin, "%lu", crap );
      allTerms++;
      terms[(n%mod)]++;

      if( crap % mod == 0 )
      {
         allDivis++;
         numDivis[(n%mod)]++;
      }

   }

   /*
   percentage = (double)allDivis/(double)allTerms;
   printf("\nb_%lu(n) = 0 (mod %lu)\t\t%5.3lf\t%5.3lf\t%5.3lf", k, mod, 100*percentage, (double)100/(double)mod, percentage*(double)mod );


   for( n=0; n < mod; n++ )
   {
      percentage = (double)numDivis[n]/(double)terms[n];
      printf("\nb_%lu(%lun+%lu) ", k, mod, n);
      printf("= 0 (mod %lu)\t\t%5.3lf\t%5.3lf\t%5.3lf", mod, 100*percentage, (double)100/(double)mod, percentage*(double)mod );
   }
   */

   printf("\n");
   fclose(fp);
   return 0;
}

