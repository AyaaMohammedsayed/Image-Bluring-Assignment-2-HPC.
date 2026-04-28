#!/bin/bash

INPUT_DIR="/app/images"
OUTPUT_DIR="/app/output"
HOSTFILE="/app/hostfile"
EXECUTABLE="./build/mpi_cv_blur"

mkdir -p $OUTPUT_DIR

declare -a TIMES
declare -a NAMES

echo "Starting Processing of 5 Images..."
echo "===================================================="

for img in "$INPUT_DIR"/*.{jpg,png}; do
    filename=$(basename "$img")
    
    echo "Processing: $filename ..."

    output=$(mpirun --allow-run-as-root -quiet \
        --hostfile "$HOSTFILE" \
        --mca btl tcp,self \
        --mca btl_tcp_if_exclude lo,docker0 \
        --mca oob_tcp_if_exclude lo,docker0 \
        --mca btl_tcp_skip_reachable_check 1 \
        --mca btl_tcp_check_ifaddr 0 \
        -np 10 \
        "$EXECUTABLE" --image "$img" --kernel 25 --mode mirror)

    exec_time=$(echo "$output" | grep "Average Execution Time" | grep -oP '\d+\.\d+')
    
    TIMES+=("$exec_time")
    NAMES+=("$filename")

    if [ -f "$OUTPUT_DIR/comparison.png" ]; then
        mv "$OUTPUT_DIR/comparison.png" "$OUTPUT_DIR/blurred_$filename"
    fi

    echo "Finished $filename in $exec_time seconds."
    echo "----------------------------------------------------"
done

echo -e "\n\n===================================================="
echo -e "FINAL PERFORMANCE REPORT"
echo -e "===================================================="
printf "%-30s | %-15s\n" "Image Name" "Avg Time (sec)"
echo "----------------------------------------------------"

total_sum=0
count=${#TIMES[@]}

for i in "${!NAMES[@]}"; do
    printf "%-30s | %-15s\n" "${NAMES[$i]}" "${TIMES[$i]}"
    total_sum=$(echo "$total_sum + ${TIMES[$i]}" | bc)
done

avg_all=$(echo "scale=4; $total_sum / $count" | bc)

echo "----------------------------------------------------"
echo "Grand Total Average for $count images: $avg_all seconds"
echo "===================================================="