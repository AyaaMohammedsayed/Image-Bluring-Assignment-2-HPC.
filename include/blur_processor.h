#ifndef BLUR_PROCESSOR_H
#define BLUR_PROCESSOR_H

#include <vector>
#include <string>

void apply_blur(const std::vector<float>& input, std::vector<float>& output, 
                int rows, int cols, int filter_size, const std::string& mode);

#endif