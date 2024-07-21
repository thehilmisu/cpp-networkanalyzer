CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lpcap

# Directories
BUILD_DIR = build

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TARGET = $(BUILD_DIR)/network_analyzer

all: ${OBJ}
	$(CXX) $(CXXFLAGS) *.o $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) *.o *.pcap

.PHONY: all clean