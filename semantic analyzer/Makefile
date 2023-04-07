CC = gcc

BUILD_DIR= ./build
OUTPUT_BINARY_NAME= lex.out
INPUT_FILE= input.c
LEX_FILE= ../lex.l

all: buildDir
	cd $(BUILD_DIR) && flex $(LEX_FILE)
	$(CC) $(BUILD_DIR)/lex.yy.c -o $(BUILD_DIR)/$(OUTPUT_BINARY_NAME)
	$(BUILD_DIR)/$(OUTPUT_BINARY_NAME) input.c -o $(BUILD_DIR)/example_output.txt

clean:
	rm -rf $(BUILD_DIR)

buildDir:
	mkdir -p $(BUILD_DIR)