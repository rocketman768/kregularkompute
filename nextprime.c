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
#include "algorithms.h"

int main(int argc, char *argv[])
{
   unsigned long number;
   int i;
   int j;

   number = strtoul(argv[1], NULL, 10);
   j = strtoul( argv[2], NULL, 10 );

   for( i=1; i <= j; ++i )
   {
      number = nextprime(number);
      printf("%d: %ld\n", i, number );
   }

   return 0;
}

