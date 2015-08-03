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

#ifndef _MODNUM_H
#define _MODNUM_H

#include <mpi.h>

//===============Global Variables=================
unsigned long int _sum_modulus;
MPI_Op SUM_MOD_OP;
MPI_Datatype mnum_type;

//================Structures==================
typedef struct{
   unsigned long int num;
   unsigned long int mod;
} ModNum;

//================Functions===================
void sumModOpInit(unsigned long int modulus);

#endif // _MODNUM_H

