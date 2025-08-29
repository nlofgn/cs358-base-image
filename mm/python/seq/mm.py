# mm.py #

#
# Matrix multiplication implementation, computing C=A*B where A and B
# are NxN matrices. The resulting matrix C is therefore NxN.
#
import os


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
    for i in range(N):
        for j in range(N):
            for k in range(N):
                C[i][j] += (A[i][k] * B[k][j])
  
    #
    # return result matrix:
    #
    return C
