############################################
                # TESTING

echo "-----------------------------------------------------------------------------------------------"
echo "RUNNING TESTS"
echo "-----------------------------------------------------------------------------------------------"


rm config.h src/config.h

NUM_PROCS=5
EPSILON=20
INTERVAL=10
DELTA=1
ALPHA=2
MAX_OFFSET_BITS=5
MAX_OFFSETS=5

echo "#ifndef CONFIG_H" >> config.h
echo "#define CONFIG_H" >> config.h
echo "" >> config.h
echo "#define NUM_PROCS $NUM_PROCS" >> config.h
echo "#define EPSILON $EPSILON" >> config.h
echo "#define INTERVAL $INTERVAL" >> config.h
echo "#define DELTA $DELTA" >> config.h
echo "#define ALPHA $ALPHA" >> config.h
echo "#define MAX_OFFSET_BITS $MAX_OFFSET_BITS" >> config.h
echo "#define MAX_OFFSETS $MAX_OFFSETS" >> config.h
echo "" >> config.h
echo "#endif" >> config.h

mv config.h src/

echo "config.h generated successfully."

make clean
make

./rc_test_run

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