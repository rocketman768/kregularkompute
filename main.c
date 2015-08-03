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
#include <math.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <mpi.h>
#include "algorithms.h"

#define FIRSTPRIME 1000000000 // One-billion. If you change this, you have to
                              //  change the estimate on number of primes required.

#define VERSIONSTRING "KregularKompute - Version 1.0 released 2008\n"

void printUsage()
{
   printf( "\nUsage:\n\t main --k num --max num --prefix string [--lastmodulus num --maxmodulus num]" );
   printf( "\nComputes 'k'-regular numbers mod prime from 0 to 'max'");
   printf( " modulo the primes > 'lastmodulus' and <= 'maxmodulus' if both present.\n");
   printf( "Otherwise, starts moduluo first prime > 1 billion and goes" );
   printf( " until the prime is high enough to reconstruct all values with Chinese Remainder Theorem.\n");
   printf( "Stores output files in the directory given by 'prefix'.\n" );
}

int main(int argc, char *argv[])
{
   unsigned long int k, high, lastmodulus, maxmodulus;
   int c, has_k=0, has_high=0, has_prefix=0, has_last_modulus=0, has_max_modulus=0;
   time_t beg, end, beg_thisiter, end_thisiter;
   double d_numprimes;
   unsigned long ui_numprimes, thisprime, i;
   int option_index;
   int manualMode = 0;
   char tmpfilename[100];
   char *outputprefix;
   char *endptr;
   int myrank, size, ret=0;
   // Define the options we allow on the command line.
   struct option long_options[] = {
      {"k", required_argument, &has_k, 1},
      {"max", required_argument, &has_high, 1},
      {"prefix", required_argument, &has_prefix, 1},
      {"lastmodulus", required_argument, &has_last_modulus, 1},
      {"maxmodulus", required_argument, &has_max_modulus, 1},
      {"version", no_argument, NULL, 0},
      {"usage", no_argument, NULL, 0},
      {0,0,0,0} // Always have to have a zero element at end.
   };

   // Setup environment.
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );

   if( myrank == 0 )
      fprintf( stdout, "\nKregularKompute - Copyright 2008 Philip Greggory Lee.\n" );

   // Initialize and prepare to Kcompute!
   ret = init();
   if( ret < 0 )
   {
      fprintf( stderr, "main(): failed to initialize environment.\n" );
      MPI_Finalize();
      return ret;
   }

   //==================BEGIN: get options===================
   while( 1 )
   {
      option_index = 0;
      c = getopt_long_only( argc, argv, "", long_options, &option_index );

      // If c is -1, then we're done with arguments.
      if( c == -1 )
         break;

      switch( c )
      {
         case 0:
           errno = 0; // Watch for string conversion errors.
           switch( option_index )
           {
              case 0:
                 k = strtoul( optarg, &endptr, 10 );
                 break;
              case 1:
                 high = strtoul( optarg, &endptr, 10 );
                 break;
              case 2:
                 outputprefix = optarg;
                 break;
              case 3:
                 lastmodulus = strtoul( optarg, &endptr, 10 );
                 break;
              case 4:
                 maxmodulus = strtoul( optarg, &endptr, 10 );
                 break;
              case 5:
                 if( myrank == 0 )
                    printf( VERSIONSTRING );
                 teardown();
                 die();
                 break;
              case 6:
                 if( myrank == 0 )
                    printUsage();
                 teardown();
                 die();
                 break;
           } // end switch( option_index )

           // errno was set or didn't find any convertable chars.
           if( errno != 0 || endptr == optarg )
           {
              // Tell the user about what went wrong.
              if( myrank == 0 )
              {
                 fprintf( stderr, "You entered strange numbers: %s\n", strerror(errno) );
                 fprintf( stderr, "Option: %s, Value: %s\n", long_options[option_index].name, optarg );
              }
              teardown();
              die();
           }
           break;
         case '?':
            if( myrank == 0 )
               fprintf( stderr, "What the heck? Strange options...\n" );
            break;
         default:
            if( myrank == 0 )
               fprintf( stderr, "Tried to get option, but returned %d\n", c );
            break;
      } // end switch( c )
   }

   // Make sure all the mandatory options are present.
   if( !(has_k & has_high & has_prefix) )
   {
      if( myrank == 0 )
         fprintf( stderr, "You are missing some mandatory options.\n" );
      teardown();
      die();
   }

   // Make sure we have BOTH optional arguments if we have at least one.
   if( has_last_modulus & has_max_modulus )
   {
      manualMode = 1;
   }
   else if( has_last_modulus ^ has_max_modulus ) // Has one but not the other
   {
      if( myrank == 0 )
         fprintf( stderr, "You must provide BOTH 'lastmodulus' and 'maxmodulus'.\n" );
      teardown();
      die();
   }
   else
      ;

   //====================END: get options=====================

   // Get estimate on how many primes we need
   // based on the Hardy-Ramanujan asymptotic
   // formula for p(n).
   // We are also assuming the primes are >=
   // 1 x 10^9, so make sure that happens in
   // the program.
   d_numprimes = M_PI / ( 9 * log(10) ) * sqrt( (2 * high) / 3 );
   ui_numprimes = (unsigned long)ceil(d_numprimes);
   thisprime = ( manualMode == 1 ) ? lastmodulus : FIRSTPRIME;

   if( myrank == 0 )
      fprintf( stderr, "Iterations: %lu\n", ui_numprimes);

   beg = time(NULL);

   // If manualMode is engaged (user picked the range of moduli)...
   if( manualMode )
   {
      for( thisprime = nextprime(thisprime); thisprime <= maxmodulus; thisprime = nextprime(thisprime) )
      {
         makeFilename( tmpfilename, k, thisprime, high, outputprefix );
      
         beg_thisiter = time(NULL);
         kregular( k, high, thisprime, tmpfilename );
         end_thisiter = time(NULL);

         if( myrank == 0 )
            fprintf( stderr, "Time elapsed for modulus=%lu: %lu\n", thisprime, (end_thisiter-beg_thisiter) );
      } // end for( thisprime <= maxmodulus )
   }
   else
   {
      for( i=0; i < ui_numprimes; ++i )
      {
         thisprime = nextprime( thisprime );
         makeFilename( tmpfilename, k, thisprime, high, outputprefix );
      
         beg_thisiter = time(NULL);
         kregular( k, high, thisprime, tmpfilename );
         end_thisiter = time(NULL);

         if( myrank == 0 )
            fprintf( stderr, "Time elapsed for modulus=%lu: %lu\n", thisprime, (end_thisiter-beg_thisiter) );
      } // end for( i < ui_numprimes )
   } // end if( manualMode )..else

   end = time(NULL);

   #ifdef DEBUG
   fprintf( stderr, "Process %d out of main for loop.\n", myrank );
   #endif
   
   if( myrank == 0 )
      fprintf( stderr, "\nTotal time elapsed: %lu\n", (end-beg));

   #ifdef DEBUG
   fprintf( stderr, "Process %d about to tear down.\n", myrank );
   #endif

   teardown();

   #ifdef DEBUG
   fprintf( stderr, "Process %d tore down.\n", myrank );
   #endif

   MPI_Finalize();
   return ret;
}

