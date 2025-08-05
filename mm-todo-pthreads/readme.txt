NOTE: this is the sequential version. The goal is to use pthreads to safely parallelize. Focus on "mm.cpp", and use pthreads library to parallelize (think fork-join). Some sample code is shown to get one thread up and running; you'll want to generalize this code based on the # of threads T. The required header "pthread.h" has already been #included.

###############################################

To build debug or optimized version:

  make debug => mm

  make opt   ==> mm-o

To run:

  mm [-?] [-n MatrixSize] [-t NumThreads]

  mm-o [-?] [-n MatrixSize] [-t NumThreads]