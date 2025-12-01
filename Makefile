CXX := g++
CC := gcc
TARGET := build/main
SRC_DIR := src/sources
OBJ_DIR := build/obj
RES_DIR := src/resources

CXXFLAGS := -std=c++20 -Isrc/headers -w
CFLAGS := -Isrc/headers -w
LIBS := -lglfw -lGL -ldl -lpthread -lX11

CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
C_SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SRCS)) \
	$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SRCS))

.PHONY: all run clean resources

all: $(TARGET)

$(TARGET): $(OBJS) | resources
	$(CXX) $(OBJS) -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

resources:
	@mkdir -p build/resources
	cp -r $(RES_DIR)/ build/resources/

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) build/resources
