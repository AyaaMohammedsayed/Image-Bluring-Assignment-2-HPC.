#ifndef MPI_MANAGER_H
#define MPI_MANAGER_H

#include <vector>
void init_mpi(int* argc, char*** argv, int& rank, int& size);
void distribute_data(const std::vector<float>& my_matrix, std::vector<float>& local_chunk, int N, int size, int rank);
#endif