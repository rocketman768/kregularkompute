# KregularKompute

A parallelized (MPI) algorithm to compute the k-regular partition numbers from
analytic number theory.

## Compiling

To compile everything, you will need "make", a C compiler, and "mpicc"
installed. You will also need the GMP library available at gmplib.org as of
March 2008. Also, this code is probably unix-specific. If you use something
other than "gcc" as a C compiler, change "CC = gcc" in Makefile to represent
what compiler you have. Simply type "make" at the command line in the
KregularKompute directory and two executables will be built: "main" & "viewer".
If you have pdflatex installed, you will find more documentation in the Doc/
directory.

## Caveats

Don't even think about running main if "prime" <= "number of terms". Why?
Algorithm does f_{n} = n^{-1} * \sum_{j=0}^{n-1}(f[n]*g[n-j]) \pmod{p}.
So, the modular inverse of n isn't good when n is 0 mod p.

