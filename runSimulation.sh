#!/bin/bash

make clean
rm debug/*

make

./hvc_test_run

delta=100
epsilon=500
alpha=25
interval=50
run_up_to=10000
number_of_processes=64

date=$(date +%Y-%m-%d)
DEBUG=debug/debug-$date.csv
OUT=debug/out-$date.csv

touch $DEBUG
touch $OUT
echo "offsize,cousize,num_procs,epsilon,interval,delta,alpha,num_events,offsetscount,counterscount,maxOffset,maxCounter"

# for(( epsilon = 100; epsilon <= 600; epsilon += 50 ))
# do
#     ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT
# done
# epsilon=500

# for(( interval = 2; interval <= $epsilon; interval *= 2 ))
# do
#     ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT
# done
# interval=50

# for(( delta = 2; delta <= $epsilon; delta *= 2 ))
# do
#     ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT
# done
# delta=100

# for(( alpha = 25; alpha <= 100; alpha *= 2 ))
# do
#     ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT
# done
 
# echo "-----------------------------------------------------------------------------------------------------------------"


for(( epsilon = 100; epsilon <= 1000; epsilon += 50 ))
do
    for(( interval = 1; interval <= $epsilon; interval ++ ))
    do
        if (( interval * epsilon == 1000 )); then
            for(( delta = 1; delta <= $epsilon; delta *= 2 ))
            do
                for(( alpha = 1; alpha <= 32; alpha *= 2 ))
                do
                    ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT
                done
            done
        fi
    done    
done

# ./hvc_run 32 100 16 200 25 1000 temp.csv temp.csv
# ./hvc_run 32 100 16 200 50 1000 temp.csv temp.csv
# ./hvc_run 32 100 16 200 100 1000 temp.csv temp.csv