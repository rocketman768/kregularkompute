OBJS = main.o dump.o gcompute.o inverse.o kregular.o prime.o sum_mod_op.o
MAINOBJS = main.o gcompute.o inverse.o kregular.o makefilenames.o misc.o prime.o sum_mod_op.o
THM4OBJS = chinesert_standalone.o thm4reconst.o
OPTS = -Wall -O2
ARCH =
LIBS = -lm -lgmp
THM4LIBS = -lgmp
CC = gcc
MPICC = mpicc

all: main viewer documentation

main: $(MAINOBJS) Makefile
	$(MPICC) $(MAINOBJS) $(OPTS) $(ARCH) -o main $(LIBS)

nextprime: prime.o nextprime.c
	$(CC) nextprime.c prime.o -o nextprime $(LIBS)

viewer: dump.o viewer.o
	$(CC) dump.o viewer.o -o viewer

thm4reconst: $(THM4OBJS) Makefile
	$(CC) -g $(THM4OBJS) -o thm4reconst $(THM4LIBS)

documentation: Doc/codeDoc.tex Makefile
	pdflatex Doc/codeDoc.tex

chinesert_standalone.o: Makefile chinesert.c
	$(CC) -DCRT_STANDALONE $(OPTS) -c chinesert.c -o chinesert_standalone.o

.c.o: $< Makefile
	$(MPICC) $(OPTS) $(ARCH) -c $<

clean:
	@rm -f *.o main viewer thm4reconst
