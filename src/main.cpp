#include <mpi.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "../include/cli.h"
#include "../include/constant.h"
#include "../include/data_manager.h"
#include "../include/mpi_manager.h"
#include "../include/blur_processor.h"

int main(int argc, char **argv)
{
    
    int rank, size;
    init_mpi(&argc, &argv, rank, size);
    UserArgs request = parse_user_input(argc, argv);
    srand(time(0));

    const int N = 10000; 
    std::vector<float> my_matrix;
    std::vector<float> local_chunk;

    // if (rank == 0)
    // {
    //     std::cout << "Master: Generating matrix..." << std::endl;
    //     my_matrix = generate_matrix(N, N);
    //     std::cout << "Master: Matrix generated with size: " << my_matrix.size() << std::endl;
    // }
    if (rank == 0)
{
    cv::Mat img = cv::imread(request.image_path, cv::IMREAD_GRAYSCALE);
    
    if (img.empty()) {
        std::cerr << "Master: Could not open or find the image!" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    cv::resize(img, img, cv::Size(N, N));

    cv::Mat img_float;
    img.convertTo(img_float, CV_32FC1, 1.0 / 255.0);

    my_matrix.assign((float*)img_float.datastart, (float*)img_float.dataend);
    
    std::cout << "Master: Image loaded and converted. Size: " << N << "x" << N << std::endl;
}
    
    distribute_data(my_matrix, local_chunk, N, size, rank);
    std::cout << "Rank " << rank << " received " << local_chunk.size() << " elements." << std::endl;

    int filter_size = request.kernel_size;
    std::string mode = request.mode;
    int rows_per_proc = local_chunk.size() / N;
    std::vector<float> blurred_chunk;

 
    double start_time = MPI_Wtime();
    apply_blur(local_chunk, blurred_chunk, rows_per_proc, N, filter_size, mode);
    double end_time = MPI_Wtime();
    double local_duration = end_time - start_time;

    std::vector<double> all_times;
    if (rank == 0) all_times.resize(size);

    MPI_Gather(&local_duration, 1, MPI_DOUBLE,
               all_times.data(), 1, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

  
    if (rank == 0)
    {
        double total_time = 0;
        std::cout << "\n========================================" << std::endl;
        std::cout << "Individual Process Execution Times:" << std::endl;
        for (int i = 0; i < size; ++i)
        {
            std::cout << "Rank " << i << " took: " << std::fixed << std::setprecision(6)
                      << all_times[i] << " seconds." << std::endl;
            total_time += all_times[i];
        }
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Average Execution Time: " << (total_time / size) << " seconds." << std::endl;
        std::cout << "========================================" << std::endl;
    }

    std::vector<float> final_blurred_matrix;
    if (rank == 0) final_blurred_matrix.resize(N * N);

    MPI_Gather(blurred_chunk.data(), local_chunk.size(), MPI_FLOAT,
               final_blurred_matrix.data(), local_chunk.size(), MPI_FLOAT,
               0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        std::cout << "Master: Gathered chunks. Saving comparison image..." << std::endl;

        
        cv::Mat final_mat(N, N, CV_32FC1, final_blurred_matrix.data());
        cv::Mat final_mat_8u;
        final_mat.convertTo(final_mat_8u, CV_8UC1, 255.0);


        cv::Mat original_mat(N, N, CV_32FC1, my_matrix.data());
        cv::Mat original_mat_8u;
        original_mat.convertTo(original_mat_8u, CV_8UC1, 255.0);

  
        cv::Mat comparison_mat;
        cv::hconcat(original_mat_8u, final_mat_8u, comparison_mat);

        cv::imwrite("/app/output/comparison.png", comparison_mat);
        std::cout << "Master: Result saved as '/app/output/comparison.png'" << std::endl;    
        std::_Exit(0);
    }
    

 
    MPI_Finalize();
    return 0;
}