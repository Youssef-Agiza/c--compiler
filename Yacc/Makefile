CC = g++

BUILD_DIR= ./build
OUTPUT_BINARY_NAME= yacc.out
INPUT_FILE= input.c
LEX_FILE= ../lex.l
YACC_FILE = ../yacc.y
SOURCES = $(BUILD_DIR)/lex.yy.c $(BUILD_DIR)/y.tab.c main.cpp

all: buildDir
	cd $(BUILD_DIR) && flex $(LEX_FILE) && yacc $(YACC_FILE)
	$(CC) -g $(SOURCES) -o $(BUILD_DIR)/$(OUTPUT_BINARY_NAME)

# $(BUILD_DIR)/$(OUTPUT_BINARY_NAME) input.c -o $(BUILD_DIR)/example_output.txt
clean:
	rm -rf $(BUILD_DIR)

buildDir:
	mkdir -p $(BUILD_DIR)