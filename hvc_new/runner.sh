# Execution method: python3 traffic_sim.py NUM_NODES UNIT EPSILON EPOCH_INTERVAL SEND_THRESHOLD
# python3 traffic_sim.py 8 1 100 10 0.007 400 800

timeout_time=60
NUM_NODES=32
EPSILON=10
INTERVAL=1
ALPHA=5
DELTA=1
UNIT=1000

rm LOGDIR/*


# for EPSILON in {5..25..5}
# do
#     echo '------------------------------------------------------------------------------'
#     echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
#     echo '------------------------------------------------------------------------------'
#     timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
# done

# EPSILON=10

# for DELTA in {1..10..1}
# do
#     echo '------------------------------------------------------------------------------'
#     echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
#     echo '------------------------------------------------------------------------------'
#     timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
# done

# DELTA=1

# for INTERVAL in {8..10..1}
# do
#     echo '------------------------------------------------------------------------------'
#     echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
#     echo '------------------------------------------------------------------------------'
#     timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
# done

# INTERVAL=1

# for ALPHA in {5..25..5}
# do
#     echo '------------------------------------------------------------------------------'
#     echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
#     echo '------------------------------------------------------------------------------'
#     timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
# done

date=$(date '+%Y-%m-%d')
sim_type='random_sim'

echo 'epsilon,perceived_epsilon,delta,interval,alpha,max_epoch,max_offset_size,mean_offset_size,percentile90_offset_size,percentile95_offset_size,percentile99_offset_size,max_counter,max_counter_size,mean_counter_size
' | tee LOGDIR/run_results_$date-$sim_type.csv

for EPSILON in {18..20..2}
do  
    x=$(( $EPSILON ))
    echo $x
    for (( DELTA=1; DELTA<=$x; DELTA++ ))
    do
        for (( INTERVAL=1; INTERVAL<=$x; INTERVAL++ ))
        do
            for ALPHA in {5..20..5}
            do
                echo '------------------------------------------------------------------------------'
                echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
                echo '------------------------------------------------------------------------------'
                timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
                python3 parse_file.py LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log | tee -a LOGDIR/run_results_$date-$sim_type.csv
                rm LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
            done
        done
    done
done
