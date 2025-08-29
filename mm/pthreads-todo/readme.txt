NOTE: this is the sequential version. The goal is to use pthreads to safely parallelize. Focus on "mm.cpp", and use pthreads library to parallelize (think fork-join). Some sample code is shown to get one thread up and running; you'll want to generalize this code based on the # of threads T. The required header <pthread.h> has already been #included.

Approach:
  1. create threads array of size T
  2. create T threads: 
      a. personaliz info object per thread
      b. pthread_create(&threads[i], nullptr /*no attr*/, mm, info);
  3. wait for T threads to finish: pthread_join(threads[i], nullptr);
  4. free threads array

###############################################

To build debug or optimized version:

  make debug => mm

  make opt   ==> mm-o

To run:

  mm [-?] [-n MatrixSize] [-t NumThreads]

  mm-o [-?] [-n MatrixSize] [-t NumThreads]