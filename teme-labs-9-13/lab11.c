#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <mpi/mpi.h>
#include <time.h>

#define P 16 
#define ROOT 0
#define ALEGERE 1
#define LIDER 2
#define STATUT_LIDER 1
#define STATUT_NONLIDER 0

#define TRUE 1
#define FALSE 0


void generare_ID(int *x, int n)
{
    int i, j, id;
    int exista;

    x[0] = rand() % 100; // setam primul element

    for (i = 1; i < n; ++i)
    {
        id = rand() % 100; // generam un ID
        exista = TRUE;

        
        while (exista)
        {
            exista = FALSE;
            for (j = 0; j < i; ++j)
            {
                if (x[j] == id)
                {
                    exista = TRUE;
                    id = rand() % 100; 
                    break;
                }
            }
        }

        x[i] = id; //stocam id ul unic
    }
}

void afisare_vector(int *x, int dim)
{
    int i;
    printf("[");
    for (i = 0; i < dim - 1; ++i)
    {
        printf("%d | ", x[i]);
    }
    printf("%d]\n\n", x[dim - 1]);
}

int main(int argc, char *argv[]) {
    int *vector_IDs; // vectorul cu ID-uri diferite, generate aleator

    int rank; 
    int coordonate[] = {-1 ,-1};
    int n; //nr procese
    MPI_Status status;
    int stanga; // vecinii procesului curent
    int dreapta;
    int runda = ALEGERE; 
    int ID; // ID-ul generat aleator
    int mesaj; 
    int statut = STATUT_LIDER; 
    int i, j; 

    MPI_Init(&argc, &argv);

    MPI_Comm commCart;
    int nrDimensiuni = 2; 
    const int dims[] = {1, P}; 
    const int periods[] = {0, 1}; 
    MPI_Cart_create(MPI_COMM_WORLD, nrDimensiuni, dims, periods, 1, &commCart);

    // rank-ul procesului curent
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // nr de procese
    MPI_Comm_size(MPI_COMM_WORLD, &n);

    if (rank == ROOT) 
    {
        // alocam memorie pentru vectorul de ID-uri
        vector_IDs = (int*) malloc(n * sizeof(int));
        if (!vector_IDs)
        {
            fprintf(stderr, "Eroare la alocarea memoriei!\n");
            MPI_Finalize();
            exit(EXIT_FAILURE);
        }

        // generam ID-uri pt fiecare proces
        srand(time(NULL));
        generare_ID(vector_IDs, n);

        printf("ID-urile de procese generate sunt:\n");
        afisare_vector(vector_IDs, n);
    }

    MPI_Scatter(vector_IDs, 1, MPI_INT, &ID, 1, MPI_INT, ROOT, commCart);

    MPI_Cart_shift(commCart, 1, 1, &stanga, &dreapta);
    MPI_Send(&ID, 1, MPI_INT, dreapta, runda, commCart);

    int gasit_lider = FALSE;
    while(!gasit_lider)
    {
        MPI_Recv(&mesaj, 1, MPI_INT, stanga, MPI_ANY_TAG, commCart, &status);

        switch(status.MPI_TAG) 
        {
            case ALEGERE: 
                if (ID == mesaj)
                {
                    statut = STATUT_LIDER;
                    runda = LIDER;
                    MPI_Send(&ID, 1, MPI_INT, dreapta, runda, commCart);
                    gasit_lider = TRUE;
                }
                else if (ID > mesaj) // daca ID-ul curent este mai mare ca cel primit, trimitem ID-ul primit spre dreapta
                {
                    MPI_Send(&mesaj, 1, MPI_INT, dreapta, runda, commCart);
                }
                break;
            case LIDER: 
                statut = STATUT_NONLIDER;
                runda = LIDER;
                MPI_Send(&mesaj, 1, MPI_INT, dreapta, runda, commCart);

                gasit_lider = TRUE; 
                break;
        }
    }

    if (statut == STATUT_LIDER)
    {
        printf("[Proces %d cu ID-ul %d] Lider!\n", rank, ID);
    }
    else
    {
        printf("[Proces %d cu ID-ul %d] Nu sunt lider.\n", rank, ID);
    }

    if (rank == ROOT) // dealocarea de memorie
    {
        if (vector_IDs)
        {
            free(vector_IDs);
        }
    }
    MPI_Finalize();

    return 0;
}
