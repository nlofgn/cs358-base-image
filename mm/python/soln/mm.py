# mm.py #

#
# Matrix multiplication implementation, computing C=A*B where A and B
# are NxN matrices. The resulting matrix C is therefore NxN.
#
import os
import concurrent.futures

# helper function to actually multiply the selected sections of the matrix
def doMM(id, A, B, C, N, start, end):

    print(f"thread {id}: starting")

    for i in range(start, end):
        for j in range(N):
            for k in range(N):
                C[i][j] += (A[i][k] * B[k][j])
    # return C because of separate memory pools, need to put matrices back together
    return C

#
# MatrixMultiply:
#
# Computes and returns C = A * B, where matrices are NxN. No attempt is made
# to optimize the multiplication.
#
def MatrixMultiply(A, B, N, T):
  
    #
    # Setup:
    #
    print(f"Num cores: {os.cpu_count()}")
    print(f"Num threads: {T}")
    print()

    #
    # Initialize target matrix in prep for summing:
    #
    C = [[0 for _ in range(N)] for _ in range(N)]
    
    #
    # For every row i of A and column j of B:
    #
    blockSize = N / T
    blockSize = int(blockSize)

    if (blockSize * T) == N: # divided evenly
        extra = 0
    else:
        # last thread will do the extra rows:
        extra = N % T

    results = []

    # use processpoolexecutor to multiply matrices
    with concurrent.futures.ProcessPoolExecutor(max_workers=T) as executor:
        for i in range(T):
             startRow = i * blockSize
             endRow = startRow + blockSize

             if (i+1) == T: # last thread does any extra rows:
                 endRow = endRow + extra

             c = executor.submit(doMM, i, A, B, C, N, startRow, endRow)
             results.append(c)

    # use results to create C matrix
    for i in range(T):
        start = i * blockSize
        end = start + blockSize

        if (i + 1 == T):
            end = end + extra
        
        for j in range(start, end):
            for k in range(N):
                C[j][k] = results[i].result()[j][k]

    #
    # return result matrix:
    #
    return C
