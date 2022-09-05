#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include"mpi.h"
#include<time.h>

#define ROOT 0

void afisarev(int *v, int dim)
{
	int i;
	printf("[");
	for(i=0; i<dim-1; ++i)
	{
		printf("%d", v[i]);
	}
}

int main(int argc, char *argv[])
{
	int vectorid[9];
	int rank;
	int n;
	MPI_Status status;
	int runda;
	int id;
	int maxid;
	int d=3;
	int i,j;
	MPI_Comm commGraph;
	MPI_Init(&argc, &argv);

	int index[]={4,6,9,12,15,18,21,25,28};
	int edges[]={1,4,5,6, 0,6, 4,7,8, 5,6,8, 0,2,7, 0,3,7, 0,1,3, 2,4,5,8, 2,3,7};

	MPI_Graph_create(MPI_COMM_WORLD,9,index,edges,0,&commGraph);

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	MPI_Comm_size(MPI_COMM_WORLD,&n);

	/*if(rank==ROOT)
	{
		vectorid[0]=0;
		vectorid[1]=1;
		vectorid[2]=2;
		vectorid[3]=3;
		vectorid[4]=4;
		vectorid[5]=5;
		vectorid[6]=6;
		vectorid[7]=7;
		vectorid[8]=8;
	
	}*/
	id=rank;
	
	int neighbours_count;
	MPI_Graph_neighbors_count(commGraph, rank, &neighbours_count);

	int* neighbours=(int*)malloc(neighbours_count * sizeof(int));
	if(!neighbours)
	{
		fprintf(stderr, "Eroare la alocare!\n");
		MPI_Comm_free(&commGraph);
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}
		
	MPI_Graph_neighbors(commGraph,rank,neighbours_count,neighbours);

	int m[neighbours_count];
	if(!m)
	{
		fprintf(stderr, "Eroare la alocare!\n");
		MPI_Comm_free(&commGraph);
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}

	///ALG FLOODMAX

	maxid=id;
	for(i=0; i<neighbours_count; ++i)
	{
		MPI_Send(&id, 1, MPI_INT, neighbours[i],0,commGraph);
	}
	for(i=0; i<d; ++i)
	{
		for(j=0; j<neighbours_count; ++j)
		{
			MPI_Recv(m+j, 1, MPI_INT, MPI_ANY_SOURCE,0,commGraph,&status);
		}
		for(j=0; j<neighbours_count; ++j)
		{
			if(maxid < m[j])
			{
				maxid=m[j];
			}
		}
		for(j=0; j<neighbours_count; ++j)
		{
			MPI_Send(&maxid,1,MPI_INT,neighbours[j],0,commGraph);
		}
	}
		if(maxid==id)
		{
			printf("Proces %d cu id-ul %d: LIDER\n",rank,id);
		}
		else
		{
			printf("Proces %d cu id-ul %d: NONLIDER\n",rank,id);
		}
		MPI_Finalize();

		return 0;
}
