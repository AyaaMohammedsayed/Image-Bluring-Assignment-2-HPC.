#include "../include/mpi_manager.h"
#include <vector>
#include <mpi.h>
#include "../include/mpi_manager.h"
#include <vector>
#include <iostream>
void init_mpi(int* argc, char*** argv, int& rank, int& size) {
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
}


void distribute_data(const std::vector<float>& full_matrix, std::vector<float>& local_chunk, int N, int size, int rank) {
    int rows_per_proc = N / size;
    int remainder = N % size;

    std::vector<int> sendcounts(size);
    std::vector<int> displs(size);

    int current_displ = 0;
    for (int i = 0; i < size; i++) {
        
        int rows = rows_per_proc + (i < remainder ? 1 : 0);

        sendcounts[i] = rows * N;
        displs[i] = current_displ;
  
        current_displ += sendcounts[i];
    }

    local_chunk.resize(sendcounts[rank]);

    MPI_Scatterv(
        full_matrix.data(),    
        sendcounts.data(),    
        displs.data(),         
        MPI_FLOAT,             
        local_chunk.data(),    
        sendcounts[rank],     
        MPI_FLOAT, 
        0,                     
        MPI_COMM_WORLD
    );
}