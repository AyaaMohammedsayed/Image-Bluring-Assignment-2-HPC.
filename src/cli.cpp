#include "../include/cli.h"
 

UserArgs parse_user_input(int argc, char* argv[]) {
    UserArgs args;
  
    for (int i = 1; i < argc; i++) {
        std::string flag = argv[i];
        if (flag == "--image") args.image_path = argv[++i];
        else if (flag == "--kernel") args.kernel_size = std::stoi(argv[++i]);
        else if (flag == "--mode") args.mode = argv[++i];
}
    return args;
}