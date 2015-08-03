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
#include <mpi.h>
#include "algorithms.h"
#include "modnum.h"

// Returns < 0 on failure, 0 on success.
int init()
{
   //=========inverse.c=========
   _remainder = malloc( sizeof(unsigned long int) * 3 );
   _quotient = malloc( sizeof(unsigned long int) * 3 );
   _auxiliary = malloc( sizeof(long int) * 3 );

   if( _remainder == NULL || _quotient == NULL || _auxiliary == NULL )
   {
      fprintf(stderr, "\ninit(): memory error\n");
      return -1;
   }
   
   //=========chinesert.c=========
   mpz_init(_M);
   mpz_init(_Mi);
   mpz_init(_yi);
   mpz_init(_ztemp);
   
   //=========kregular.c==========
   f = NULL;
   g = NULL;
   all = NULL;

   return 0;
}

void teardown()
{
   //============inverse.c=============
   free(_remainder);
   free(_quotient);
   free(_auxiliary);

   //=========chinesert.c=========
     mpz_clear(_M);
   mpz_clear(_Mi);
   mpz_clear(_yi);
   mpz_clear(_ztemp);
   _M_calculated = 0;
   
   //=========kregular.c==========
   if( f != NULL )
      free(f);
   if( g != NULL )
      free(g);
   // I am not freeing all b/c I never malloc-d it.
}

void die()
{
   MPI_Finalize();
   exit(-1);
}

