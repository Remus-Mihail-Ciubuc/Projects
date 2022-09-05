#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <mpi/mpi.h>

#define MPI_COMM_WORLD OMPI_PREDEFINED_GLOBAL( MPI_Comm, ompi_mpi_comm_world)
#define P 16
#define ROOT 0

void afisare_matr(int **m, int n)
{
    int i, j;
    for (i = 0; i < n; ++i)
    {
        for (j = 0; j < n; ++j)
        {
            printf("%4d ", m[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int **A, **B; 
    int **C; 
    int *mem_A, *mem_B, *mem_C; // zonele de memorie

    int rank; 
    int coordonate[] = {-1 ,-1}; 
    int n; 
    MPI_Status status; 
    int vecin_sus; 
    int vecin_jos;
    int vecin_stanga;
    int vecin_dreapta;
    int eticheta = 99; 
    int i, j; 

    int M1, M2; 
    int M3; 
    int rez;

    MPI_Init(&argc, &argv);


    MPI_Comm old_comm = MPI_COMM_WORLD;
    MPI_Comm commCart;
    int dimensiuni = 2; 
    int nr_elemD = (int)sqrt(P); 
    const int dims[] = {nr_elemD, nr_elemD}; 
    const int periods[] = {1, 1}; 
    MPI_Cart_create(old_comm, dimensiuni, dims, periods, 1, &commCart);

    // rank-ul procesului curent
    MPI_Comm_rank(commCart, &rank);

    // numarul de procese
    MPI_Comm_size(commCart, &n);

    if (n != P) 
    {
        fprintf(stderr, "Numarul de procese trebuie sa fie egal cu %d!\n", P);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    if (rank == ROOT) 
    {

        A = (int**) malloc(nr_elemD * sizeof(int*));
        B = (int**) malloc(nr_elemD * sizeof(int*));
        C = (int**) malloc(nr_elemD * sizeof(int*));
        if (!A || !B || !C)
        {
            fprintf(stderr, "Eroare la alocarea memoriei!\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

        // alocam memorie pentru retinerea elementelor matricelor
        mem_A = (int*) malloc(nr_elemD * nr_elemD * sizeof(int));
        mem_B = (int*) malloc(nr_elemD * nr_elemD * sizeof(int));
        mem_C = (int*) malloc(nr_elemD * nr_elemD * sizeof(int));
        if (!mem_A || !mem_B || !mem_C)
        {
            fprintf(stderr, "Eroare la alocarea memoriei!\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

       
        A[0] = mem_A;
        B[0] = mem_B;
        C[0] = mem_C;
        for (i = 1; i < nr_elemD; ++i)
        {
            A[i] = mem_A + i * nr_elemD;
            B[i] = mem_B + i * nr_elemD;
            C[i] = mem_C + i * nr_elemD;
        }

        // initializam matricile cu valori
        A[0][0] = 1; A[0][1] = 2; A[0][2] = 3; A[0][3] = 4;
        A[1][0] = 6; A[1][1] = -1; A[1][2] = -6; A[1][3] = 2;
        A[2][0] = -3; A[2][1] = -4; A[2][2] = 8; A[2][3] = 11;
        A[3][0] = 3; A[3][1] = 1; A[3][2] = 6; A[3][3] = -11;

        B[0][0] = 6; B[0][1] = 1; B[0][2] = -10; B[0][3] = -5;
        B[1][0] = -7; B[1][1] = -4; B[1][2] = 9; B[1][3] = 7;
        B[2][0] = 9; B[2][1] = 1; B[2][2] = 2; B[2][3] = -3;
        B[3][0] = 1; B[3][1] = 5; B[3][2] = 3; B[3][3] = 4;

        memset(mem_C, 0, nr_elemD * nr_elemD * sizeof(int)); // in matricea finala punem numai zerouri

        printf("Matricile initiale:\n");
        printf("A = \n");
        afisare_matr(A, nr_elemD);
        printf("\n");
        printf("B = \n");
        afisare_matr(B, nr_elemD);
        printf("\n");
    }

    
    MPI_Cart_coords(commCart, rank, dimensiuni, coordonate);
    i = coordonate[0];
    j = coordonate[1];

   
    MPI_Scatter(mem_A, 1, MPI_INT, &M1, 1, MPI_INT, ROOT, commCart);
    MPI_Scatter(mem_B, 1, MPI_INT, &M2, 1, MPI_INT, ROOT, commCart);

   
    int k;
    MPI_Cart_shift(commCart, 1, 1, &vecin_stanga, &vecin_dreapta);
    printf("[Proces %d] Vecinul din stanga este: %d & Vecinul din dreapta este: %d\n", rank, vecin_stanga, vecin_dreapta);

    for (k = 0; k < i; ++k) 
    {
        
        MPI_Send(&M1, 1, MPI_INT, vecin_stanga, eticheta, commCart);
        printf("[AI - Proces %d] Elementul \"%d\" din matricea A  a fost trimis catre vecinul din stanga %d.\n", rank, M1, vecin_stanga);

       
        MPI_Recv(&M1, 1, MPI_INT, vecin_dreapta, eticheta, commCart, &status);
        printf("[AI - Proces %d] Elementul \"%d\" din matricea A a fost primit de la vecinul din dreapta %d.\n", rank, M1, vecin_dreapta);
    }


    MPI_Cart_shift(commCart, 0, 1, &vecin_sus, &vecin_jos);
    printf("[Proces %d] Vecinul de sus este : %d & Vecinul de jos este : %d\n", rank, vecin_sus, vecin_jos);

    for (k = 0; k < j; ++k) 
    {
        
        MPI_Send(&M2, 1, MPI_INT, vecin_sus, eticheta, commCart);
        printf("[AI - Proces %d] Elementul \"%d\" din matricea B a fost trimis catre vecinul de sus %d.\n", rank, M1, vecin_sus);

        MPI_Recv(&M2, 1, MPI_INT, vecin_jos, eticheta, commCart, &status);
        printf("[AI - Proces %d] Elementul \"%d\" din matricea B a fost primit de la vecinul de jos %d.\n", rank, M1, vecin_jos);
    }

    //algoritmul lui Canon

    M3 = 0;
    for (k = 0; k < nr_elemD - 1; ++k)
    {
        M3 += M1 * M2;

       
        MPI_Send(&M1, 1, MPI_INT, vecin_stanga, eticheta, commCart);
        printf("[CANNON - Proces %d] Elementul \"%d\" din matricea A a fost trimis catre vecinul din stanga %d.\n", rank, M1, vecin_stanga);

        
        MPI_Recv(&M1, 1, MPI_INT, vecin_dreapta, eticheta, commCart, &status);
        printf("[CANNON - Proces %d] Elementul \"%d\" din matricea A a fost primit de la vecinul din dreapta %d.\n", rank, M1, vecin_dreapta);

       
        MPI_Send(&M2, 1, MPI_INT, vecin_sus, eticheta, commCart);
        printf("[CANNON - Proces %d] Elementul \"%d\" din matricea B a fost trimis catre vecinul de sus %d.\n", rank, M1, vecin_sus);

      
        MPI_Recv(&M2, 1, MPI_INT, vecin_jos, eticheta, commCart, &status);
        printf("[CANNON - Proces %d] Elementul \"%d\" din matricea B a fost primit de la vecinul de jos %d.\n", rank, M1, vecin_jos);
    }

    rez = M3 + M1 * M2; 

  
    MPI_Gather(&rez, 1, MPI_INT, mem_C, 1, MPI_INT, ROOT, commCart);

    if (rank == ROOT) 
        printf("\nMatricea rezultat:\n");
        printf("C = \n");
        afisare_matr(C, nr_elemD);
        printf("\n");

        

        if (mem_A)
        {
            free(A[0]);
            free(A);
        }
        if (mem_B)
        {
            free(B[0]);
            free(B);
        }
        if (mem_C)
        {
            free(C[0]);
            free(C);
        }
    }


    MPI_Finalize();

    return 0;
}
