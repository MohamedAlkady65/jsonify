CC      = gcc


BUILD_PATH = build

SRC     = $(wildcard src/*.c)
TARGET  = $(BUILD_PATH)/jsonify
CFLAGS =  -Ilib

LIB_SRC = $(filter-out src/main.c,$(SRC))
UNITY_SRC = $(wildcard lib/unity/src/*.c)

TESTS_SRC     = tests/test_main.c $(LIB_SRC) $(UNITY_SRC)
TESTS_TARGET  = $(BUILD_PATH)/test_runner
TESTS_FLAGS =  -Ilib -Isrc -Ilib/unity/src -DUNITY_INCLUDE_DOUBLE

DEBUG_FLAGS = -g3 -O0

.PHONY: build run test test-debug clean

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: build
	./$(TARGET)

test:
	$(CC) $(TESTS_FLAGS) $(TESTS_SRC)  -o $(TESTS_TARGET)
	./$(TESTS_TARGET)

test-debug:
	mkdir -p $(BUILD_PATH)
	$(CC) $(TESTS_FLAGS) $(DEBUG_FLAGS) $(TESTS_SRC) -o $(TESTS_TARGET)

clean:
	rm -rf $(BUILD_PATH)
