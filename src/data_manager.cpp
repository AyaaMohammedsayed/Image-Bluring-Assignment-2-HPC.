#include "../include/data_manager.h"
#include <vector>
#include <cstdlib>
#include <ctime>



std::vector<float> generate_matrix(int rows, int cols) {
    std::vector<float> matrix(rows * cols);
    
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = static_cast<float>(rand() % 2); 
    }
    
    return matrix;
}