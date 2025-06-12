#!/bin/bash


gcc -fopenmp -o phil phil.c
#  clang -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib -I/opt/homebrew/opt/libomp/include -lomp phil.c -o omptest2


for threads in 1 2 4 8
do
    
    for schedule in 0 1 2
    do
        echo "==========================================="
        echo "Running with $threads threads and schedule $schedule"
        if [ "$schedule" -eq 0 ]; then
            echo "Schedule type: static"
        elif [ "$schedule" -eq 1 ]; then
            echo "Schedule type: dynamic"
        elif [ "$schedule" -eq 2 ]; then
            echo "Schedule type: guided"
        fi
        echo "==========================================="

        ./phil $threads $schedule
        echo ""
    done
done