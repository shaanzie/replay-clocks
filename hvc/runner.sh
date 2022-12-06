# Execution method: python3 traffic_sim.py NUM_NODES UNIT EPSILON EPOCH_INTERVAL SEND_THRESHOLD
# python3 traffic_sim.py 8 1 100 10 0.007 400 800

timeout_time=600
XMIT_MIN=2 
XMIT_MAX=2
SEND_THRESHOLD=0.005
NUM_NODES=32
UNIT=1
# EPSILON=160
INTERVAL=100

rm LOGFILES/*

for EPSILON in {1..100..5}
do
    echo '---------------------------------------------------------'
    echo $EPSILON
    echo '---------------------------------------------------------'
    sleep 3
    timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT $EPSILON*1000 $INTERVAL $SEND_THRESHOLD $XMIT_MIN*1000 $XMIT_MAX*1000
done