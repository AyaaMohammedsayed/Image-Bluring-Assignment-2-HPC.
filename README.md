#  Very Large Image Blur using OpenMPI

##  Overview
This project implements a high-performance parallel image blurring system designed to process very large-scale images (up to 10,000 × 10,000 pixels). The system leverages C++ with OpenMPI to distribute computation efficiently across a Docker-based multi-node cluster.

---

## Key Features
- Parallel image processing using OpenMPI
- Supports very large images (10K resolution)
- Multiple blur filter configurations
- Distributed computation across a multi-subnet Docker cluster
- Automated benchmarking and reporting
- Scalable architecture for HPC experimentation

---

##  Core Concept
The system applies a blur filter (convolution) by:
1. Splitting images into chunks
2. Distributing chunks across nodes
3. Processing in parallel
4. Reassembling results

---

##  Infrastructure & Networking

### Architecture
- Subnet 1: 172.25.0.0/24 → Master, Worker1, Worker2
- Subnet 2: 172.26.0.0/24 → Worker3, Worker4

### Master Node Role
- Scheduler
- Gateway between subnets
- Aggregator

### Static Routing
On Master:
ip route add 172.26.0.0/24 via <Worker3_IP>

On Subnet 2 Workers:
ip route add 172.25.0.0/24 via <Master_IP>

---

##  Parameters

### Filters
- Square
- Stencil

### Sizes
3, 5, 7 ... up to 25

### Padding
- Zeros
- Mirror
- None

---

## Execution

```bash
mpirun -np 5 \
  --mca btl tcp,self \
  --mca btl_tcp_if_exclude lo,docker0 \
  --mca btl_tcp_skip_reachable_check 1 \
  --mca routed direct \
  --hostfile hostfile.txt \
  ./blur_program
```

For detailed setup and execution steps, please refer to the `run_instructions` file included in the project.

---

## Automation

Script: `run_all.sh`

This bash script runs experiments on 5 images using mirror mode with a kernel size of 25.  
It also generates a performance report based on execution over the network.



---
##  Performance Report

The following report shows execution times for each processed image along with the grand average:

![Performance Report](https://github.com/AyaaMohammedsayed/Image-Bluring-Assignment-2-HPC./blob/main/performance%20report.jpg)


---

##  Visuals
The displayed image presents a comparison between the input and the output after applying the mirror mode kernel (size 25)

![Performance Report](https://github.com/AyaaMohammedsayed/Image-Bluring-Assignment-2-HPC./blob/main/comparison.jpg)


---
##  Project Supervision & Team

### Supervision
- Dr. Sabah  
- Eng. Mohamed Abdel-Majeed (Teaching Assistant)

### Development Team

| Name           | Section | Bench No. |
|----------------|---------|-----------|
| Aya Mohamed    | 1       | 4         |
| Hanin Mustafa  | 1       | 8         |
| Nadia Kamel    | 2       | 36        |

---

**Department:** Computer Engineering  
**Project:** Senior Graduation Project 2026
Eng. Mohamed Abdel-Majeed (Teaching Assistant)
