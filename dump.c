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
#include <assert.h>

#define READNUM 262144 // One megabyte of unsigned longs.

void dump(char *file)
{
   unsigned long int terms, i, *crap;
   size_t elts_read;

   crap = malloc( sizeof(unsigned long int) * READNUM );
   assert( crap != NULL );

   FILE *fp = fopen(file, "rb");
   if( fp == NULL )
   {
      fprintf(stderr, "\ndump(): Cant open %s\n", file);
      return;
   }

   // Read header
   fread( &terms, sizeof(unsigned long int), 1, fp );

   /*
   for(i=0; i < terms; i++)
   {
      fread( &crap, sizeof(unsigned long int), 1, fp );
      printf("%lu\n", crap);
   }
   */

   do
   {
      elts_read = fread( crap, sizeof(unsigned long int), READNUM, fp );
      for( i=0; i < elts_read; i++ )
         printf("%lu\n", crap[i]);
   } while( elts_read == READNUM );

   fclose(fp);
   return;
}

