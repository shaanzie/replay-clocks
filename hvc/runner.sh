# Execution method: python3 traffic_sim.py NUM_NODES UNIT EPSILON EPOCH_INTERVAL SEND_THRESHOLD

timeout_time=60
SEND_MIN=1
SEND_MAX=12
RECV_MIN=1
RECV_MAX=13
XMIT_MIN=5000 
XMIT_MAX=20000
SEND_THRESHOLD=0.007
NUM_NODES=8
UNIT=1

# Changing EPSILON

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 100 200 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 500 200 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 1000 200 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 5000 200 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

# Changing INTERVAL

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 10000 50 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 10000 100 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 10000 1000 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX

timeout $timeout_time python3 traffic_sim.py $NUM_NODES $UNIT 10000 10000 $SEND_THRESHOLD $XMIT_MIN $XMIT_MAX