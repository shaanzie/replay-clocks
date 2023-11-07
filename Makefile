# CC=g++
# FLAGS= -std=c++17 -g

# SRC_DIR=src/
# TEST_FILES= hvc.cpp TestCase.cpp
# TEST_FILES_FULL=$(addprefix $(SRC_DIR), $(TEST_FILES))
# SRC_FILES= hvc.cpp process.cpp simulation.cpp
# SRC_FILES_FULL=$(addprefix $(SRC_DIR), $(SRC_FILES))
# HDR_FILES= hvc.h message.h process.h simulation.h config.h
# HDR_FILES_FULL=$(addprefix $(SRC_DIR), $(HDR_FILES))

# all: hvc_run hvc_test

# hvc_run: $(HDR_FILES_FULL) $(SRC_FILES_FULL)
# 	$(CC) $(FLAGS) $(SRC_FILES_FULL) -o hvc_run

# hvc_test: $(HDR_FILES_FULL) $(TEST_FILES_FULL)
# 	$(CC) $(FLAGS) $(TEST_FILES_FULL) -o hvc_test_run

# clean:
# 	rm hvc_run hvc_test_run

CC=g++
FLAGS= -std=c++17 -g

SRC_DIR=src/
TEST_FILES= rc.cpp TestCaseRC.cpp
TEST_FILES_FULL=$(addprefix $(SRC_DIR), $(TEST_FILES))
SRC_FILES= rc.cpp TestCaseRC.cpp
SRC_FILES_FULL=$(addprefix $(SRC_DIR), $(SRC_FILES))
HDR_FILES= rc.h config.h
HDR_FILES_FULL=$(addprefix $(SRC_DIR), $(HDR_FILES))

all: rc_run rc_test

rc_run: $(HDR_FILES_FULL) $(SRC_FILES_FULL)
	$(CC) $(FLAGS) $(SRC_FILES_FULL) -o rc_run

rc_test: $(HDR_FILES_FULL) $(TEST_FILES_FULL)
	$(CC) $(FLAGS) $(TEST_FILES_FULL) -o rc_test_run

clean:
	rm rc_run rc_test_run