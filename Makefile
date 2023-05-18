CC=g++
FLAGS= -std=c++17 -g

SRC_DIR=src/
SRC_FILES= hvc.cpp TestCase.cpp
SRC_FILES_FULL=$(addprefix $(SRC_DIR), $(SRC_FILES))
HDR_FILES= hvc.h
HDR_FILES_FULL=$(addprefix $(SRC_DIR), $(HDR_FILES))

all: hvc_run

hvc_run: $(HDR_FILES_FULL) $(SRC_FILES_FULL)
	$(CC) $(FLAGS) $(SRC_FILES_FULL) -o hvc_run

clean:
	rm hvc debug/hvc_debug*