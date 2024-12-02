#!/bin/bash

# Settings
TEST_FILE="./test-file"
OUTPUT_CSV="results.csv"
TRIALS=5
declare -A GRANULARITIES=(
    ["4KB"]=$((4 * 1024))
    ["8KB"]=$((8 * 1024))
    ["16KB"]=$((16 * 1024))
    ["32KB"]=$((32 * 1024))
    ["64KB"]=$((64 * 1024))
    ["128KB"]=$((128 * 1024))
    ["256KB"]=$((256 * 1024))
    ["512KB"]=$((512 * 1024))
    ["1MB"]=$((1 * 1024 * 1024))
    ["2MB"]=$((2 * 1024 * 1024))
    ["4MB"]=$((4 * 1024 * 1024))
    ["8MB"]=$((8 * 1024 * 1024))
    ["16MB"]=$((16 * 1024 * 1024))
    ["32MB"]=$((32 * 1024 * 1024))
    ["64MB"]=$((64 * 1024 * 1024))
    ["128MB"]=$((128 * 1024 * 1024))
)

READABLE_GRANULARITIES=(
    "4KB"
    "8KB"
    "16KB"
    "32KB"
    "64KB"
    "128KB"
    "256KB"
    "512KB"
    "1MB"
    "2MB"
    "4MB"
    "8MB"
    "16MB"
    "32MB"
    "64MB"
    "128MB"
)

TEST_SIZE=$((1024 * 1024 * 1024))  # 1GB

echo "Test,Operation,Granularity,Granularity(Human-Readable),Stride,Trial,Time(ms)" > "$OUTPUT_CSV"

run_test() {
    local test_type=$1
    local operation=$2

    for human_readable in "${READABLE_GRANULARITIES[@]}"; do
        granularity=${GRANULARITIES[$human_readable]}
        for ((trial = 1; trial <= TRIALS; trial++)); do
            echo "Running $test_type ($operation), Granularity: ${human_readable} ($granularity bytes), Trial: $trial"

            # Build arguments based on the test type
            local test_args="-d $TEST_FILE -t $TEST_SIZE -g $granularity"
            if [ "$test_type" == "random" ]; then
                test_args+=" -l 0 -u $TEST_SIZE"
            fi
            if [ "$operation" == "write" ]; then
                test_args+=" -w"
            fi

            result=$(./mbench "$test_type" $test_args | grep -oE '[0-9]+')

            # Append to CSV
            echo "$test_type,$operation,$granularity,$human_readable,0,$trial,$result" >> "$OUTPUT_CSV"
        done
    done
}

for test_type in "size" "random"; do
    for operation in "read" "write"; do
        run_test $test_type $operation
    done
done

# Section for stride

declare -A STRIDE_GRANULARITIES=(
    ["4KB"]=$((4 * 1024))
    ["16KB"]=$((16 * 1024))
    ["64KB"]=$((64 * 1024))
    ["256KB"]=$((256 * 1024))
    ["2MB"]=$((2 * 1024 * 1024))
    ["8MB"]=$((8 * 1024 * 1024))
)

READABLE_STRIDE_GRANULARITIES=(
    "4KB"
    "16KB"
    "64KB"
    "256KB"
    "2MB"
    "8MB"
)

    for operation in "read" "write"; do
        for human_readable_stride in "${READABLE_STRIDE_GRANULARITIES[@]}"; do
            stride_size=${STRIDE_GRANULARITIES[$human_readable]}
            for human_readable in "${READABLE_GRANULARITIES[@]}"; do
                granularity=${GRANULARITIES[$human_readable]}
            for ((trial = 1; trial <= TRIALS; trial++)); do
                echo "Running stride ($operation), Stride: ${human_readable_stride} ($granularity bytes), Trial: $trial"
                # Build arguments based on the test type
                local test_args="-d $TEST_FILE -t $TEST_SIZE -g $granularity"
                if [ "$operation" == "write" ]; then
                    test_args+=" -w"
                fi

                result=$(./mbench stride $test_args | grep -oE '[0-9]+')

                # Append to CSV
                echo "$test_type,$operation,$granularity,$human_readable,1,$trial,$result" >> "$OUTPUT_CSV"
            done
        done