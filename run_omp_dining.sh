#!/bin/bash

gcc -fopenmp -o phil phil.c

# Çıktı dosyasını temizle
echo "OPENMP DINING PHILOSOPHERS TEST RESULTS" > results.txt
echo "=======================================" >> results.txt

# Thread sayıları
for threads in 1 2 4 8
do
    # Scheduling türleri: 0=static, 1=dynamic, 2=guided
    for schedule in 0 1 2
    do
        echo "===========================================" | tee -a results.txt
        echo "Running with $threads threads and schedule $schedule" | tee -a results.txt
        if [ "$schedule" -eq 0 ]; then
            echo "Schedule type: static" | tee -a results.txt
        elif [ "$schedule" -eq 1 ]; then
            echo "Schedule type: dynamic" | tee -a results.txt
        elif [ "$schedule" -eq 2 ]; then
            echo "Schedule type: guided" | tee -a results.txt
        fi
        echo "===========================================" | tee -a results.txt

        ./phil $threads $schedule | tee -a results.txt
        echo "" | tee -a results.txt
    done
done