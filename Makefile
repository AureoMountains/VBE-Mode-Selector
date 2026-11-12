CC = gcc

CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror \
         -Iinclude -g

SANITIZERS = -fsanitize=address,undefined

SRC = src/vbe_mode.c
TEST = tests/test_vbe_mode.c
EXAMPLE = examples/basic_usage.c

BUILD = build

.PHONY: all test example clean

all: test example

$(BUILD):
	mkdir -p $(BUILD)

test: $(BUILD)
	$(CC) $(CFLAGS) $(SANITIZERS) $(SRC) $(TEST) \
		-o $(BUILD)/test_vbe_mode
	./$(BUILD)/test_vbe_mode

example: $(BUILD)
	$(CC) $(CFLAGS) $(SRC) $(EXAMPLE) \
		-o $(BUILD)/basic_usage
	./$(BUILD)/basic_usage

clean:
	rm -rf $(BUILD)
