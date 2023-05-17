CC=g++
FLAGS= -std=c++11 -g

SRC_DIR=src/
SRC_FILES= Process.cpp  utility.cpp runsimulation.cpp main.cpp
SRC_FILES_FULL=$(addprefix $(SRC_DIR), $(SRC_FILES))
HDR_FILES= hvc.h Process.h runsimulation.h utility.h
HDR_FILES_FULL=$(addprefix $(SRC_DIR), $(HDR_FILES))

all: hvc

hvc: $(HDR_FILES_FULL) $(SRC_FILES_FULL)
	$(CC) $(FLAGS) $(SRC_FILES_FULL) -o hvc

clean:
	rm hvc debug/hvc_debug*

