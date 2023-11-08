############################################               
            # SIMULATION
############################################

rm config.h src/config.h 2> /dev/null

NUM_PROCS=$1
EPSILON=$2
INTERVAL=$3
DELTA=$4
ALPHA=$5
MAX_OFFSET_BITS=$6
MAX_OFFSETS=$7

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

# echo "config.h generated successfully."

make -s clean 2> /dev/null
make -s 2> /dev/null