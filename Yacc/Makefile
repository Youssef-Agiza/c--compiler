CC = g++

BUILD_DIR= ./build
OUTPUT_BINARY_NAME= yacc.out
INPUT_FILE= input.c
LEX_FILE= ../lex.l
YACC_FILE = ../yacc.y
SOURCES = $(BUILD_DIR)/lex.yy.c $(BUILD_DIR)/y.tab.c

all: buildDir
	cd $(BUILD_DIR) && yacc -d $(YACC_FILE) && flex $(LEX_FILE)
	$(CC) -g $(SOURCES) -o $(BUILD_DIR)/$(OUTPUT_BINARY_NAME)
	$(BUILD_DIR)/$(OUTPUT_BINARY_NAME) < input.c

clean:
	rm -rf $(BUILD_DIR)

buildDir:
	mkdir -p $(BUILD_DIR)