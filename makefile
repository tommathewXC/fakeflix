APP_NAME:=fakeflixserver
OUTPUT_FOLDER:=out
BIN_OUT:=$(OUTPUT_FOLDER)/bin
LOG_OUT:=$(OUTPUT_FOLDER)/log
SRC_FILES=src
MAIN_FILE=$(SRC_FILES)/main.cpp
CONTENT_FOLDER:=content

directory:
	mkdir -p $(BIN_OUT)

build:
	g++ -std=c++17 $(MAIN_FILE) $(SRC_FILES)/utils.cpp -o $(BIN_OUT)/$(APP_NAME)

run:
	./$(BIN_OUT)/$(APP_NAME) -logFolder $(LOG_OUT)/log.txt -contentFolder $(CONTENT_FOLDER)