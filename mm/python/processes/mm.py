# mm.py #

#
# Matrix multiplication implementation, computing C=A*B where A and B
# are NxN matrices. The resulting matrix C is therefore NxN.
#
import os
import concurrent.futures


#
# MatrixMultiply:
#
# Computes and returns C = A * B, where matrices are NxN. No attempt is made
# to optimize the multiplication.
#
def doMM(id, A, B, C, N, start, end):

    print(f"process {id}: starting")

    for i in range(start, end):
        for j in range(N):
            for k in range(N):
                C[i][j] += (A[i][k] * B[k][j])

    return C


def MatrixMultiply(A, B, N, T):
  
    #
    # Setup:
    #
    print(f"Num cores: {os.cpu_count()}")
    print(f"Num processes: {T}")
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

    #
    # fork off the processes, implicit join at the end of the with:
    #
    futures = []

    with concurrent.futures.ProcessPoolExecutor(max_workers=T) as executor:
        for i in range(T):
             startRow = i * blockSize
             endRow = startRow + blockSize

             if (i+1) == T: # last thread does any extra rows:
                 endRow = endRow + extra

             f = executor.submit(doMM, i, A, B, C, N, startRow, endRow)

             futures.append(f)

    #
    # we have to loop through the futures, obtain each result
    # matrix, and then copy those result values back into the 
    # original C matrix for return:
    #
    for i in range(T):
        startRow = i * blockSize
        endRow = startRow + blockSize

        if (i+1) == T: # last thread does any extra rows:
            endRow = endRow + extra

        f = futures[i]
        RM = f.result()  # result matrix

        for r in range(startRow, endRow):
            for c in range(N):
                C[r][c] = RM[r][c]
            
        i = i + 1

    #
    # return result matrix:
    #
    return C
