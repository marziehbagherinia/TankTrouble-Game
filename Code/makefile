CC = g++ 
CC_LIB = -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer
CC_STD = --std=c++11
CC_OPT = -c

BUILD_DIR = build
SRC_DIR = src

a.out: $(BUILD_DIR)/main.o $(BUILD_DIR)/tank.o $(BUILD_DIR)/map.o $(BUILD_DIR)/bullets.o $(BUILD_DIR)/handler.o $(BUILD_DIR)/rsdl.o
	${CC} ${CC_STD} $(BUILD_DIR)/main.o $(BUILD_DIR)/tank.o $(BUILD_DIR)/map.o $(BUILD_DIR)/bullets.o $(BUILD_DIR)/handler.o $(BUILD_DIR)/rsdl.o ${CC_LIB} -o a.out

$(BUILD_DIR)/main.o: $(SRC_DIR)/rsdl.hpp $(SRC_DIR)/main.cpp
	${CC} ${CC_STD} ${CC_OPT} $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/tank.o: $(SRC_DIR)/rsdl.hpp $(SRC_DIR)/tank.cpp
	${CC} ${CC_STD} ${CC_OPT} $(SRC_DIR)/tank.cpp -o $(BUILD_DIR)/tank.o

$(BUILD_DIR)/map.o: $(SRC_DIR)/rsdl.hpp $(SRC_DIR)/map.cpp
	${CC} ${CC_STD} ${CC_OPT} $(SRC_DIR)/map.cpp -o $(BUILD_DIR)/map.o

$(BUILD_DIR)/bullets.o: $(SRC_DIR)/rsdl.hpp $(SRC_DIR)/bullets.cpp
	${CC} ${CC_STD} ${CC_OPT} $(SRC_DIR)/bullets.cpp -o $(BUILD_DIR)/bullets.o

$(BUILD_DIR)/handler.o: $(SRC_DIR)/rsdl.hpp $(SRC_DIR)/handler.cpp
	${CC} ${CC_STD} ${CC_OPT} $(SRC_DIR)/handler.cpp -o $(BUILD_DIR)/handler.o

$(BUILD_DIR)/rsdl.o: $(SRC_DIR)/rsdl.hpp $(SRC_DIR)/rsdl.cpp
	${CC} ${CC_STD} ${CC_OPT} $(SRC_DIR)/rsdl.cpp -o $(BUILD_DIR)/rsdl.o

clean:
	rm build/*
