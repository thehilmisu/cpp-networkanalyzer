CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lpcap

BUILD_DIR = build

OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TARGET = $(BUILD_DIR)/network_analyzer

all: ${OBJ}
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) *.o $(LDFLAGS) -o $(TARGET)
	mv *.o build/

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET) *.o *.pcap
	rm -Rf build

.PHONY: all clean