CC = g++

BUILD_DIR= ./build
OUTPUT_BINARY_NAME= parser.out
INPUT_FILE= input.c
LEX_FILE= lex.l

SOURCE_FILE= parser.cpp

all: buildDir
	cd $(BUILD_DIR) && flex ../$(LEX_FILE)
	$(CC) $(BUILD_DIR)/lex.yy.c $(SOURCE_FILE) -o $(BUILD_DIR)/$(OUTPUT_BINARY_NAME)
	$(BUILD_DIR)/$(OUTPUT_BINARY_NAME) input.c -o $(BUILD_DIR)/example_output.txt

clean:
	rm -rf $(BUILD_DIR)

buildDir:
	mkdir -p $(BUILD_DIR)