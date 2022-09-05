#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi/mpi.h"

#define P 3
#define ROOT 0

int* xmalloc(int x)
{
    int* A = (int*) malloc(x * sizeof(int));
    if (!A)
    {
        fprintf(stderr, "Eroare la alocarea memoriei!\n");
        exit(EXIT_FAILURE);
    }

    memset(A, 0, (size_t)x);
    return A;
}


int main(int argc, char* argv[]) {

    int rank; 
    int i,j,k;
    int m; 

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &m);

    int A[4][4];
    int *B= xmalloc(P);

    if (m != P) 
    {
        fprintf(stderr, "Nr de procese trebuie sa fie: %d\n", P);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    if (rank == ROOT) 
    {
        

        A[0][0]=2;     A[0][1]=5;      A[0][2]=7;
        A[1][0]=9;     A[1][1]=1;      A[1][2]=4;
        A[2][0]=3;     A[2][1]=8;      A[2][2]=6;

        B[0]=23;
        B[1]=6;
        B[2]=8;

        printf("Matricea initiala A:\n");
        printf("A = \n");
        for (i = 0; i < P; ++i)
        {
            for (j = 0; j < P; ++j)
                printf("%d ", A[i][j]);
            printf("\n");
        }
        printf("\n");
        printf("Vectorul initial B:\n");
        printf("B = \n");
        for (i = 0; i < P; ++i)
            printf("%d  ", B[i]);
        printf("\n\n");
    }

    for(k=0;k<=P-1;++k)
        {
            if(rank==k)
            {
                for(j=k+1;j<=P-1;++j)
                    A[k][j]=A[k][j]/A[k][k];

                B[k]=B[k]/A[k][k];
                A[k][k]=1;
            }


            MPI_Bcast(A[k],m,MPI_INT,k,MPI_COMM_WORLD);
            MPI_Bcast(&B[k],1,MPI_INT,k,MPI_COMM_WORLD);

            if(rank>k)
            {
                
                for(j=k+1;j<=P-1;++j)
                    A[rank][j]=A[rank][j]-(A[rank][k]*A[k][j]);

                B[rank]=B[rank]-(A[rank][j]*B[k]);
                A[rank][k]=0;
            }
        }

    if (rank == ROOT)
    {
        printf("Matricea A:\n");
        printf("A = \n");
        for (i = 0; i < P; ++i)
        {
            for (j = 0; j < P; ++j)
                printf("%4d ", A[i][j]);
            printf("\n");
        }
        printf("\n");
        printf("Vectorul B:\n");
        printf("B = \n");
        for (i = 0; i < P; ++i)
            printf("%d  ", B[i]);
        printf("\n\n");

    }

    MPI_Finalize();

    return 0;
}
