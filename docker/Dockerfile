# 1. Base Image
FROM ubuntu:22.04

# 2. Set environment variables to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# 3. Install dependencies: MPI, C++, OpenCV, and its runtime libraries
RUN apt-get update && apt-get install -y \
    build-essential \
    openmpi-bin openmpi-common libopenmpi-dev \
    cmake \
    git \
    libopencv-dev \
    python3-opencv \
    pkg-config \
    libavcodec-dev libavformat-dev libswscale-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# 4. Create App directory
WORKDIR /app

# 5. Copy the code
COPY . .

# 6. Build the project using CMake (the best way for OpenCV)
RUN mkdir build && cd build && \
    cmake .. && \
    make

# 7. Run Command (NP is usually size of processes)
CMD ["mpirun", "--allow-run-as-root", "-np", "4", "./build/mpi_cv_blur"]