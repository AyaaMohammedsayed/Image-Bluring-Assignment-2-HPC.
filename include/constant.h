#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

namespace Config {
  
    const std::string INPUT_PATH= "Assigment-2/images/input_1.jpg";

    const std::string OUTPUT_PATH = "Assigment-2/output/";
}
struct UserArgs {
    std::string image_path = Config::INPUT_PATH;
    int kernel_size = 25;
    std::string mode = "zeros"; 

};

#endif
