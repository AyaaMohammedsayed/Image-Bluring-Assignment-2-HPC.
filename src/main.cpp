#include <mpi.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <filesystem>
#include <opencv2/opencv.hpp> // مكتبة OpenCV
#include "../include/data_manager.h"
#include "../include/mpi_manager.h"
#include "../include/blur_processor.h"

int main(int argc, char **argv)
{
    int rank, size;
    init_mpi(&argc, &argv, rank, size);
    srand(time(0));

    const int N = 10000;
    std::vector<float> my_matrix;
    std::vector<float> local_chunk;

    // الماستر بيولد الصورة الأصلية
    if (rank == 0)
    {
        std::cout << "Master: Generating matrix..." << std::endl;
        my_matrix = generate_matrix(N, N);
        std::cout << "Master: Matrix generated with size: " << my_matrix.size() << std::endl;
    }

    // توزيع الداتا على البروسيسز
    distribute_data(my_matrix, local_chunk, N, size, rank);

    std::cout << "Rank " << rank << " received " << local_chunk.size() << " elements." << std::endl;

    int filter_size = 3;
    std::string mode = "zeros";
    int rows_per_proc = local_chunk.size() / N;
    std::vector<float> blurred_chunk;

    // بداية قياس الوقت
    double start_time = MPI_Wtime();

    apply_blur(local_chunk, blurred_chunk, rows_per_proc, N, filter_size, mode);

    double end_time = MPI_Wtime();
    double local_duration = end_time - start_time;

    // تجميع أوقات التنفيذ
    std::vector<double> all_times;
    if (rank == 0)
    {
        all_times.resize(size);
    }

    MPI_Gather(&local_duration, 1, MPI_DOUBLE,
               all_times.data(), 1, MPI_DOUBLE,
               0, MPI_COMM_WORLD);

    // عرض نتائج الأداء (من الماستر)
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

    // --- الجزء الجديد: تجميع الصورة (Result Collection) ---
    std::vector<float> final_blurred_matrix;
    if (rank == 0)
    {
        final_blurred_matrix.resize(N * N);
    }

    // تجميع الأجزاء من كل البروسيسز
    MPI_Gather(blurred_chunk.data(), local_chunk.size(), MPI_FLOAT,
               final_blurred_matrix.data(), local_chunk.size(), MPI_FLOAT,
               0, MPI_COMM_WORLD);

    // حفظ الصورة (الماستر فقط)
    if (rank == 0)
    {
        std::cout << "Master: Gathered chunks. Saving final image..." << std::endl;

        // تحويل الـ Vector لـ Mat عشان OpenCV
        cv::Mat final_mat(N, N, CV_32FC1, final_blurred_matrix.data());

        // حفظ الصورة (الضرب في 255 لتحويل قيم الـ Float لـ Pixel values)
        cv::imwrite("../output/final_blurred.png", final_mat * 255.0);
        std::cout << "Master: Result saved as 'final_blurred.png'" << std::endl;
    }

    MPI_Finalize();
    return 0;
}