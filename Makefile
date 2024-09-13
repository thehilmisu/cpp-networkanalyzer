CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lpcap -lncurses

BUILD_DIR = build

# Include source files from the current directory and ui/ directory
SOURCES := $(wildcard *.cpp ui/*.cpp)

# Generate object files list
OBJ := $(SOURCES:.cpp=.o)

TARGET = $(BUILD_DIR)/network_analyzer

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@
	# Move object files to build directory
	mv *.o $(BUILD_DIR)/
	mv ui/*.o $(BUILD_DIR)/

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) *.o ui/*.o *.pcap
	rm -Rf $(BUILD_DIR)

.PHONY: all clean
