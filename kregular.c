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
#include <unistd.h>
#include <signal.h>
#include "mpi.h"
#include "modnum.h"
#include "algorithms.h"

#define ALARM_INTERVAL 10 // Time between updates is 10 seconds.

unsigned long int *_terms, *_n; // Only used to communicate with the update() function.

void update(int x)
{
   printf("\nkregular(): terms remaining: %ld", *_terms - *_n );
   alarm( ALARM_INTERVAL );
}

void kregular(unsigned long int k, unsigned long int terms, unsigned long int modulus, char *outfile)
{

   unsigned long int n, j, node, mysum, lastval;
   unsigned long long int crap;
   int tag=1, myrank, size;
   unsigned long int answer;
   FILE *out;

   #ifdef DEBUG
   unsigned long long unreducedsum;
   time_t start, end;
   #endif

   // Gotta initialize the reduction operator
   // so it knows how to add numbers together.
   sumModOpInit( modulus );

   MPI_Status status;
   MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   // malloc() g if we haven't already.
   if( g == NULL )
   {
      g = malloc( sizeof(unsigned long) * (terms+1) );
      if( g == NULL )
      {
         fprintf(stderr, "\n\tkregular(): Couldn't allocate memory for g\n");
         die();
      }
   }

   // Generate the "g"s
   #ifdef DEBUG
   start = time(NULL);
   #endif

   gcompute( g, k, terms, modulus );

   #ifdef DEBUG
   end = time(NULL);
   printf("gcompute: Prime %lu Process %d, %lus\n", modulus, myrank, (end-start));
   #endif
   
   if( myrank == 0 )
   {
      out = fopen(outfile, "w");

      if( out == NULL )
      {
         fprintf(stderr, "\n\tkregular(): Could not open file %s\n", outfile);
         die();
      }

      // Do some stuff to let our update() function know
      // what we're doing.
      _terms = &terms;
      _n = &n;

      #ifdef KREG_UPDATES
         // Install update() as the alarm handler and set alarm.
         if( myrank == 0 )
         {
            signal( SIGALRM, update );
            alarm(ALARM_INTERVAL);
         }
      #endif

   }

   // Allocate space for f if we haven't already.
   if( f == NULL )
      f = malloc( (terms/size + 1) * sizeof(unsigned long int) );

   // Make sure no memory errors
   if( f == NULL || g == NULL )
   {
      fprintf(stderr, "\n\tkregular(): Memory allocation error.\nf: %p g: %p \n", f, g);
      die();
   }

   // Set base case for recursion
   if( myrank == 0 )
      f[0] = 1;

   for( n=1; n <= terms; n++ )
   {
      // Compute my own partial sum
      mysum = 0;
      #ifdef DEBUG
      unreducedsum = 0;
      #endif
      for( j=myrank, lastval=0; j <=n-1; j+=size )
      {
         crap = (unsigned long long)f[lastval++]*(unsigned long long)g[n-j] % modulus;
         #ifdef DEBUG
         unreducedsum += crap;
         #endif
         crap += mysum;
         mysum = crap % modulus;
      }

      answer = 0;

      #ifdef DEBUG
      if( n == 91 )
      {
         fprintf(stderr, "unreducedsum=%llu\nmysum=%lu\ninverse=%lu\n\n", unreducedsum, mysum, inverse(n,modulus) );
         //for( j=1; j <= 91; ++j )
         //   fprintf(stderr,"g[%lu]=%lu\n", j, g[j]);
      }
      #endif

      // Sum up all the mysums modulo modulus.
      // Send the result to machine (n%size).
      MPI_Reduce( &mysum, &answer, 1, MPI_UNSIGNED_LONG, SUM_MOD_OP, (n%size), MPI_COMM_WORLD );

      // If I'm supposed to store the answer, store it.
      if( n%size == myrank )
      {
         crap = (unsigned long long)(answer)*(unsigned long long)inverse(n, modulus);
         // From the above for() loop, lastval now represents the first "empty"
         // position in f[]. So, this is where we put the next answer.
         f[lastval] = (unsigned long)(crap%modulus);

         #ifdef DEBUG
         fprintf( stderr, "%d %lu\n", myrank, n );
         #endif
      }

   }

   // Done computing. Collect data and write it out.
   if( myrank == 0 )
   {
      #ifdef KREG_UPDATES
         // Stop updates.
         alarm(0);
      #endif

      #ifdef DEBUG
      printf("\nCollecting data...\n");
      #endif

      // Write header
      fwrite( &terms, sizeof(unsigned long int), 1, out );

      // Pretend all[] and g[] are different :)
      all = g;

      // Copy node 0's values into all[]
      for(j=1; j <= lastval; j++)
         all[j*size] = f[j];

      // I'm gonna use f as a bucket
      // for the other nodes' answers.

      // Get results from other nodes and interleave them into all[]
      for( node=1; node < size; node++ )
      {
         MPI_Recv( &lastval, 1, MPI_UNSIGNED_LONG, node, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
         MPI_Recv( f, lastval+1, MPI_UNSIGNED_LONG, node, MPI_ANY_TAG, MPI_COMM_WORLD,&status );

         for( j=0; j < lastval; j++ )
         {
            //printf("\nLastval: %lu", lastval);
            //printf("\nWriting into all[%lu]", j*size+node);
            all[j*size+node] = f[j];
         }
      }

      #ifdef DEBUG
      printf("Writing data...\n");
      #endif

      fwrite( &all[1], sizeof(unsigned long int), terms, out );
      fclose(out);

      #ifdef DEBUG
      printf("...done\n");
      #endif
   }
   else
   {
      MPI_Send( &lastval, 1, MPI_UNSIGNED_LONG, 0, tag, MPI_COMM_WORLD );
      MPI_Send( f, lastval+1, MPI_UNSIGNED_LONG, 0, tag, MPI_COMM_WORLD );
   }

   #ifdef DEBUG
   printf("Process %d about to return.\n", myrank);
   #endif

   return;
}

