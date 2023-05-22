clear

make clean
rm debug/*

make

./hvc_test_run

delta=20
epsilon=500
alpha=0.25
interval=200
run_up_to=10000
number_of_processes=100

date=$(date +%Y-%m-%d)
DEBUG=debug/debug-$date.csv
OUT=debug/out-$date.csv

touch $DEBUG
touch $OUT
echo "offsize,cousize,num_procs,epsilon,interval,delta,alpha"

for(( epsilon = 1; epsilon<=600; epsilon*=2 ))
do
    for(( interval = 1; interval <= $epsilon; interval *= 2 ))
    do
        for(( delta = 1; delta <= $interval; delta   *= 2 ))
        do
            for(( alpha = 25; alpha <= 100; alpha *= 2 ))
            do
                ./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT
            done
        done
    done    
done