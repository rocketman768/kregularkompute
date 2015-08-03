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

#ifndef _ALGORITHMS_H
#define _ALGORITHMS_H

#include <gmp.h>

//==============Global variables==============
/***Variables for inverse.c***/
unsigned long int *_remainder, *_quotient;
long int *_auxiliary;

/***Variables for chinesert.c***/
mpz_t _M, _Mi, _yi, _ztemp;
int _M_calculated;

/***Variables for kregular.c***/
unsigned long int *f, *g, *all;

//=================Functions==================
/* chinesert.c */
void crt( mpz_t x, unsigned long int *a, unsigned long int *m, unsigned long int elts );

/* gcompute.c */
void gcompute( unsigned long int *ret, unsigned long int k, unsigned long int high, unsigned long int modulus);

/* inverse.c */
unsigned long int inverse(unsigned long int number, unsigned long int modulus);

/* kregular.c */
void kregular( unsigned long k, unsigned long int terms, unsigned long int modulus, char *outfile );

/* prime.c */
int isprime( unsigned long int n );
unsigned long int nextprime( unsigned long int n );

/* dump.c */
void dump( char *file );

/* misc.c */
int init();
void teardown();
void die();

/* makefilenames.c */
void makeFilename( char *name, unsigned long k, unsigned long modulus, unsigned long high, char *prefix );

#endif // _ALGORITHMS_H

