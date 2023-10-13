MPICC = mpicc.mpich

OMPFLAGS = -fopenmp
OMPLIBS = -lgomp

CFLAGS = -g -std=gnu99 -I. -Wall $(OMPFLAGS)
LDLIBS = $(OMPLIBS) -lm

# add path to gtmp and gtmpi
GTMP_PATH = ../omp
GTMPI_PATH = ../mpi
# add source for gtmp and gtmpi
GTMP_SRC:= $(GTMP_PATH)/gtmp2.c #tournament
GTMPI_SRC:= $(GTMPI_PATH)/gtmpi1.c #dessemination barrier

all: combined 

SRCS := $(wildcard *.c) $(GTMP_SRC) $(GTMPI_SRC)
OBJS = $(SRCS:.c=.o)

%.o: %.c
	$(MPICC) -c $(CFLAGS) $< -o $@

combined: $(OBJS) 
	$(MPICC) -o $@ $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS)

clean:
	rm -rf *.o *.dSYM combined

