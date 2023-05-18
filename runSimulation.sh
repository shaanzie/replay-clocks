clear

make clean
rm debug/*

make

./hvc_test_run

delta=100
epsilon=500
alpha=0.05
interval=200
run_up_to=10000
number_of_processes=100

date=$(date +%Y-%m-%d)
DEBUG=debug/debug-$date.csv
OUT=debug/out-$date.csv

touch $DEBUG
touch $OUT
echo "Timestep,offsize,cousize" | tee -a $OUT

echo "---------------------------------------------------------------------------------------------------"
echo "Running for E:$epsilon, I:$interval, D:$delta, A:$alpha for N:$number_of_processes and T:$run_up_to"
echo "---------------------------------------------------------------------------------------------------"

./hvc_run $number_of_processes $epsilon $interval $delta $alpha $run_up_to $DEBUG $OUT