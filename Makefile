CC=g++
CFLAGS=-std=c++17 -Wall -g
TAR_BALL=files.tar
TAR_FLAGS=-cvf

# The name of the source file that contains the hardcoded test cases
SIMPLE_TEST_MAIN=main.cpp

# The source files we use for building custom_tests
ALL_SRC=$(wildcard *.cpp)

# The source files that we use for building instructor_tests
INSTRUCTOR_TEST_SRC=$(filter-out $(SIMPLE_TEST_MAIN), $(ALL_SRC))

# The name of the resulting executable
APP=test

custom_tests:
	$(CC) $(CFLAGS) $(ALL_SRC) -o $(APP)	

instructor_tests:
	$(CC) $(CFLAGS) $^ $(INSTRUCTOR_TEST_SRC) -o $(APP)	

tar:
	tar $(TAR_FLAGS) $(TAR_BALL) $(shell find . -type f)

clean:
	rm -f $(APP)
	rm -f *.tar
