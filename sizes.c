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

int main()
{
	printf("Unsigned int: %d bytes\n", sizeof(unsigned int));
	printf("Unsigned long int: %d bytes\n", sizeof(unsigned long int));
	printf("Unsinged long long int: %d bytes\n", sizeof(unsigned long long int));

	unsigned long int a,b;
        unsigned long long int ans;
        a = 0x1 << 31;
        b = a;

	ans = (unsigned long long)a * (unsigned long long)b;
	printf("%lu %lu %llu\n", a, b, ans);

	return 0;
}

