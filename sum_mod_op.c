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

#include <mpi.h>
#include "modnum.h"

void sumMod( unsigned long int *in, unsigned long int *inout, int *len, MPI_Datatype *dptr )
{
   int i;
   unsigned long long result;

   for( i=0; i < *len; i++ )
   {
      result = *inout;
      result += *in;
      *inout = result % _sum_modulus;

      in++; inout++;
   }
}

void sumModOpInit(unsigned long int modulus)
{
   //MPI_Type_contiguous( sizeof(ModNum), MPI_PACKED, &mnum_type );
   //MPI_Type_commit( &mnum_type );
   
   _sum_modulus = modulus;
   MPI_Op_create( (MPI_User_function *)sumMod, 1, &SUM_MOD_OP );
}

