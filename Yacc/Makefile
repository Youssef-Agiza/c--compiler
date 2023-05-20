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
	@echo "\n\n"
	$(BUILD_DIR)/$(OUTPUT_BINARY_NAME) < $(INPUT_FILE)

clean:
	rm -rf $(BUILD_DIR)

buildDir:
	mkdir -p $(BUILD_DIR)