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
#include <gmp.h>
#include "algorithms.h"

#define M_ITEMS 2048 // The max number of items m[] can have.
                     // If this program is crapping out, this
                     // is probably the reason.

// Program verifies case l=ell from Theorem 4.
// Reads input from stdin about location of files.
// Format is:
//   prime file
//   prime2 file2
//   ...
// Where prime, prime2,... represent the moduli of file, file2,... .
int main(int argc, char *argv[])
{
   unsigned long int ell, i, n, num, thisprime, numprimes;
   unsigned long int three_to_ell, three_to_ellMinusOne, nbound;
   unsigned long int *a, *m;
   mpz_t answer;
   int items;
   char thisfilename[128];
   FILE **datafiles;

   if( argc < 2 )
   {
      fprintf( stderr, "\nUsage:\n\t%s ell\n", argv[0] );
      fprintf( stderr, "Makes sure that the case l=ell is true for Theorem 4 in\n`Parity of 5-regular and 13-regular Partition Functions'");
      return 1;
   }

   // Parse arguments.
   ell = strtoul(argv[1], NULL, 10);

   // Allocate memory for stuff.
   m = malloc( sizeof(unsigned long) * M_ITEMS );
   a = malloc( sizeof(unsigned long) * M_ITEMS );
   datafiles = malloc( sizeof(FILE *) * M_ITEMS );
   mpz_init(answer);

   // Set up constants.
   three_to_ell = 1;
   three_to_ellMinusOne = 1;

   for( i=0; i < ell; ++i )
      three_to_ell *= 3;
   for( i=0; i < ell-1; ++i )
      three_to_ellMinusOne *= 3;
   nbound = 7*three_to_ellMinusOne + 3;

   // Set up primes and files.
   i = 0;
   while(1)
   {
      items = fscanf( stdin, "%lu %s\n", &thisprime, thisfilename );
      if( items == 2 )
      {
         datafiles[i] = fopen( thisfilename, "r" );
         m[i] = thisprime;
         i++;
      }
      else
         break;
   }

   // Now we know how many primes we're working with.
   numprimes = i;

   for( n=1; n <= nbound; ++n )
   {
      // Set num to number in b_13 fn in thm4.
      num = three_to_ell*n + (5*three_to_ellMinusOne - 1)/2;

      // Set up the a[] array.
      for( i=0; i < numprimes; ++i )
      {
         fseek( datafiles[i], 4L*num, SEEK_SET );
         fread( &a[i], 4, 1, datafiles[i] );
      }

      // Get result from chinese remainder thm.
      crt( answer, a, m, numprimes );

      // If it's not zero mod 3...oh shit.
      if( mpz_mod_ui( answer, answer, (unsigned long)3 ) != 0 )
      {
         printf("Failed case: n=%lu num=%lu b_13(num)=", n, num);
         mpz_out_str(stdout, 10, answer);
         printf("\n");
         fflush(stdout);

         exit(1);
      }
   }

   printf("Yay! Case l=%lu is true!\n", ell);

   // Close all files.
   for( i = 0; i < numprimes; ++i )
      fclose( datafiles[i] );

   // Free all memory.
   free(a);
   free(m);
   free(datafiles);

   return 0;
}

