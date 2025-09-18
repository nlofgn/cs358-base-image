/* mm.cpp */

//
// Matrix multiplication implementation, computing C=A*B where A and B
// are NxN matrices. The resulting matrix C is therefore NxN.
//
#include <iostream>
#include <string>
#include <sys/sysinfo.h>
#include <pthread.h>

#include "alloc2D.h"
#include "mm.h"

using namespace std;

//
// struct for communicating with thread-based implementation:
//
struct ThreadInfo {
  int      ID;
  int      N;
  int      StartRow;
  int      EndRow;
  double** A;
  double** B;
  double** C;

  ThreadInfo(int id, int n, int startrow, int endrow, double** a, double** b, double** c)
   : ID(id), N(n), StartRow(startrow), EndRow(endrow), A(a), B(b), C(c)
  { }
};

static void* mm(void*);

//
// MatrixMultiply:
//
// Computes and returns C = A * B, where matrices are NxN. No attempt is made
// to optimize the multiplication.
//
double** MatrixMultiply(double** const A, double** const B, int N, int T)
{
  double** C = New2dMatrix<double>(N, N);

  //
  // Setup:
  //
  cout << "Num cores: " << get_nprocs() << endl;
  cout << "Num threads: " << T << endl;
  cout << endl;

  //
  // Initialize target matrix in prep for summing:
  //
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      C[i][j] = 0.0;

  //
  // For starters, just execute using the main thread, nothing
  // in parallel:
  //
  pthread_t* threads = new pthread_t[T];
  int blocksize = N/T;
  int extrarows = N%T;

  for (int i = 0; i < T; i++) {
    int startrow = i * blocksize;
    int endrow = startrow + blocksize;
    if (i + 1 == T) {
      endrow += extrarows;
    }

    struct ThreadInfo* info;
    info = new ThreadInfo(i /*id*/, 
                        N /*matrix size*/,
                        startrow /*start row*/, 
                        endrow /*end row*/,
                        A, B, C);
    pthread_create(&threads[i], nullptr, mm, (void*) info);
  }

  for (int i = 0; i < T; i++) {
    pthread_join(threads[i], nullptr);
  }

  delete[] threads;

  //
  // NOTE: mm() will delete the info object as part of the cleanup.
  //

  //
  // return pointer to result matrix:
  //
  return C;
}

//
// mm
//
// This function does the actual matrix multiplication, where each 
// thread does M rows, where M = N/T (size of matrix / # of threads).
// 
// When the computation is over, the info object passed will be 
// deleted.
// 
// Example: if there are 100 rows in the matrices and 4 threads, then
//   thread 0: rows 0..24
//   thread 1: rows 25..49
//   thread 2: rows 50..74
//   thread 3: rows 75..99
//
static void* mm(void* msg)
{
  struct ThreadInfo* info = (struct ThreadInfo*) msg;

  //
  // copy values out of struct so code is easier to read:
  //
  int ID = info->ID;
  int N  = info->N;
  int StartRow = info->StartRow;
  int EndRow = info->EndRow;
  double** A = info->A;
  double** B = info->B;
  double** C = info->C;

  cout << "thread " << ID << " starting" << endl;
  
  //
  // For every row i of A and column j of B:
  //
  for (int i = StartRow; i < EndRow; i++)
  {
    for (int k = 0; k < N; k++)
    {
      for (int j = 0; j < N; j++)
      {
        C[i][j] += (A[i][k] * B[k][j]);
      }
    }
  }

  //
  // free struct that was passed to us:
  //
  delete info;
  
  //
  // NOTE: the thread is not returning anything, answers are
  // in matrix C. So return nullptr.
  //
  return nullptr;
}
