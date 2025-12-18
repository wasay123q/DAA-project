CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude

# Target executable
TARGET = SCERRS

# Directories
SRC_DIR = src
OBJ_DIR = obj

# Files
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/CityGraph.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

run: all
	./$(TARGET)