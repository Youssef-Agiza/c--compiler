CC = g++

BUILD_DIR= ./build
OUTPUT_BINARY_NAME= parser.out
INPUT_FILE= input_err2.c
LEX_FILE= lex.l

SOURCE_FILE= parser.cpp interpreter.cpp


all: buildDir
	cd $(BUILD_DIR) && flex -C ../$(LEX_FILE)
	$(CC)  $(BUILD_DIR)/lex.yy.c $(SOURCE_FILE) -o $(BUILD_DIR)/$(OUTPUT_BINARY_NAME)
	@echo "\n====Running the parser===\n"
	$(BUILD_DIR)/$(OUTPUT_BINARY_NAME)  $(INPUT_FILE)


run: build
	$(BUILD_DIR)/$(OUTPUT_BINARY_NAME)  $(INPUT_FILE)


build: buildDir
	cd $(BUILD_DIR) && flex -C ../$(LEX_FILE)
	$(CC) $(BUILD_DIR)/lex.yy.c $(SOURCE_FILE) -o $(BUILD_DIR)/$(OUTPUT_BINARY_NAME)



clean:
	rm -rf $(BUILD_DIR)

buildDir:
	mkdir -p $(BUILD_DIR)