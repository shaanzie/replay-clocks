# Execution method: python3 traffic_sim.py NUM_NODES UNIT EPSILON EPOCH_INTERVAL SEND_THRESHOLD
# python3 traffic_sim.py 8 1 100 10 0.007 400 800

timeout_time=240
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

for INTERVAL in {5..10..1}
do
    echo '------------------------------------------------------------------------------'
    echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
    echo '------------------------------------------------------------------------------'
    timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
done

INTERVAL=1

# for ALPHA in {5..25..5}
# do
#     echo '------------------------------------------------------------------------------'
#     echo "Running sim with params N $NUM_NODES.E $EPSILON.I $INTERVAL.A $ALPHA.D $DELTA"
#     echo '------------------------------------------------------------------------------'
#     timeout $timeout_time python3 traffic_simulation.py $NUM_NODES $EPSILON $INTERVAL $DELTA $ALPHA $UNIT | tee LOGDIR/N$NUM_NODES.E$EPSILON.I$INTERVAL.A$ALPHA.D$DELTA.log
# done