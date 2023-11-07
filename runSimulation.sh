#!/bin/bash

clear

for(( num_procs = 32; num_procs <= 64; num_procs += 32 ))
do
    for(( epsilon = 100; epsilon <= 1000; epsilon += 50 ))
    do
        for(( interval = 1; interval <= $epsilon; interval ++ ))
        do
            if (( interval * epsilon % 1000 == 0 && interval*epsilon <= 6000 )); then
                for(( delta = 1; delta <= $epsilon; delta *= 2 ))
                do
                    for(( alpha = 1; alpha <= 32; alpha *= 2 ))
                    do
                        
                        echo "-----------------------------------------------------------------------------------------------"
                        echo "BUILD STARTED"
                        echo "-----------------------------------------------------------------------------------------------"
                        
                        bash build.sh $num_procs $epsilon $interval $delta $alpha 10 6
                        
                        echo "-----------------------------------------------------------------------------------------------"
                        echo "BUILD SUCCESS"
                        echo "-----------------------------------------------------------------------------------------------"
                        
                        echo "-----------------------------------------------------------------------------------------------"
                        echo "RUNNING TESTS"
                        echo "-----------------------------------------------------------------------------------------------"

                        # ./hvc_test_run

                        if [ $? -ne 0 ]; then
                            echo "-----------------------------------------------------------------------------------------------"
                            echo "TESTS FAILED!"
                            echo "-----------------------------------------------------------------------------------------------"
                            exit 1  # Exit the script with a non-zero status code
                        else
                            echo "-----------------------------------------------------------------------------------------------"
                            echo "TESTS SUCCEEDED!"
                            echo "-----------------------------------------------------------------------------------------------"
                        fi

                        echo "-----------------------------------------------------------------------------------------------"
                        echo "RUNNING SIMULATION"
                        echo "-----------------------------------------------------------------------------------------------"

                        echo "offsize,cousize,num_procs,epsilon,interval,delta,alpha,num_events,offsetscount,counterscount,maxOffset,maxCounter,avg_send_time,avg_recv_time,system_clock_time"

                        # ./hvc_run $num_procs $epsilon $interval $delta $alpha

                        if [ $? -ne 0 ]; then
                            echo "-----------------------------------------------------------------------------------------------"
                            echo "SIMULATION FAILED!"
                            echo "-----------------------------------------------------------------------------------------------"
                            exit 1  # Exit the script with a non-zero status code
                        else
                            echo "-----------------------------------------------------------------------------------------------"
                            echo "SIMULATION SUCCEEDED!"
                            echo "-----------------------------------------------------------------------------------------------"
                        fi
                    done
                done
            fi
        done    
    done
done