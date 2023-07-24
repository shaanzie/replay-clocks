#!/bin/bash

clear

make clean
rm debug/*

make

./hvc_test_run

delta=0
epsilon=0
alpha=0
interval=0
run_up_to=10000

number_of_processes=32

date=$(date +%Y-%m-%d)
DEBUG=debug/debug-$date.csv
OUT=debug/out-$date.csv

touch $DEBUG
touch $OUT
echo "offsize,cousize,num_procs,epsilon,interval,delta,alpha,num_events,offsetscount,counterscount,maxOffset,maxCounter" | tee -a $OUT

for(( epsilon = 100; epsilon <= 1000; epsilon += 50 ))
do
    for(( interval = 1; interval <= $epsilon; interval ++ ))
    do
        if (( interval * epsilon % 1000 == 0 && interval*epsilon <= 6000 )); then
            for(( delta = 1; delta <= $epsilon; delta *= 2 ))
            do
                for(( alpha = 1; alpha <= 32; alpha *= 2 ))
                do
                    ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT | tee -a $OUT
                done
            done
        fi
    done    
done

number_of_processes=64

for(( epsilon = 100; epsilon <= 1000; epsilon += 50 ))
do
    for(( interval = 1; interval <= $epsilon; interval ++ ))
    do
        if (( interval * epsilon % 1000 == 0 && interval*epsilon <= 6000 )); then
            for(( delta = 1; delta <= $epsilon; delta *= 2 ))
            do
                for(( alpha = 1; alpha <= 32; alpha *= 2 ))
                do
                    ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT | tee -a $OUT
                done
            done
        fi
    done    
done