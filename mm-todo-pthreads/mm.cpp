/* mm.cpp */

//
// Matrix multiplication implementation, computing C=A*B where A and B
// are NxN matrices. The resulting matrix C is therefore NxN.
//
#include <iostream>
#include <string>
#include <sys/sysinfo.h>

#include "alloc2D.h"
#include "mm.h"
#include "pthread.h"

using namespace std;

//
// struct for communicating with thread-based implementation:
//
struct ThreadInfo {
  int      ID;
  int      NumThreads;
  int      N;
  double** A;
  double** B;
  double** C;

  ThreadInfo(int id, int t, int n, double** a, double** b, double** c)
   : ID(id), NumThreads(t), N(n), A(a), B(b), C(c)
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
  struct ThreadInfo* info;
  info = new ThreadInfo(0 /*id*/, 
                        1 /*num threads*/, 
                        N /*matrix size*/,
                        A, B, C);

  mm(info);

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
  int T  = info->NumThreads;
  int N  = info->N;
  double** A = info->A;
  double** B = info->B;
  double** C = info->C;

  cout << "thread " << ID << " starting" << endl;
  
  //
  // how many rows do we multiply?
  //
  int blockSize = N / T;
  int startRow = ID * blockSize;
  int endRow = startRow + blockSize;

  // 
  // NOTE: if NumThreads does not divide evenly, the last thread
  // does the extra rows.
  //
  if (blockSize * T != N) { // did not evenly divide:
    int extra = N % T;
    
    if ((ID + 1) == T)  // last thread in the group:
      endRow += extra;
  }
  
  //
  // For every row i of A and column j of B:
  //
  for (int i = startRow; i < endRow; i++)
  {
    for (int j = 0; j < N; j++)
    {
      for (int k = 0; k < N; k++)
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
