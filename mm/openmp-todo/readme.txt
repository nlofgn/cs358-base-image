NOTE: this is the sequential version. The goal is to use openMP to safely parallelize. Focus on "mm.cpp", and use OpenMP parallel or parallel for directives to parallelize. Example:

#pragma omp parallel for num_threads(T)
for (...

Don't forget #include <omp.h> as well.

###################################################

To build debug or optimized version:

  make debug => mm

  make opt   ==> mm-o

To run:

  mm [-?] [-n MatrixSize] [-t NumThreads]

  mm-o [-?] [-n MatrixSize] [-t NumThreads]