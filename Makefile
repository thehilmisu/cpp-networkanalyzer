# Variables
CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lpcap
OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

# Directories
BUILD_DIR = build

TARGET = $(BUILD_DIR)/network_analyzer
OBJECTS = $(BUILD_DIR)/main.o

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

all: ${OBJ}
	$(CXX) $(CXXFLAGS) *.o $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) *.o *.pcap

.PHONY: all clean