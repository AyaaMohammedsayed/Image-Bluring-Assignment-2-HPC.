#include "../include/blur_processor.h"
#include <vector>
#include <string>

float get_pixel(const std::vector<float>& input, int x, int y, int rows, int cols, const std::string& mode) {

    if (x >= 0 && x < cols && y >= 0 && y < rows) {
        return input[y * cols + x];
    }

    if (mode == "zeros") return 0.0f;
    

    if (mode == "mirror") {
        int nx = (x < 0) ? -x - 1 : (x >= cols ? 2 * cols - 1 - x : x);
        int ny = (y < 0) ? -y - 1 : (y >= rows ? 2 * rows - 1 - y : y);
        return input[ny * cols + nx];
    }
    
    return 0.0f; 
}

void apply_blur(const std::vector<float>& input, std::vector<float>& output, 
                int rows, int cols, int filter_size, const std::string& mode) {
    
    output.resize(input.size());
    int radius = filter_size / 2;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            
            if (mode == "none" && (i < radius || i >= rows - radius || j < radius || j >= cols - radius)) {
                output[i * cols + j] = input[i * cols + j];
                continue;
            }

            float sum = 0;
            for (int ki = -radius; ki <= radius; ki++) {
                for (int kj = -radius; kj <= radius; kj++) {
                    sum += get_pixel(input, j + kj, i + ki, rows, cols, mode);
                }
            }
            output[i * cols + j] = sum / (filter_size * filter_size);
        }
    }
}