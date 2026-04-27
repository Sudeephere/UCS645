CXX = g++
CXXFLAGS = -O3 -std=c++14 -Wall
LDFLAGS = 

# Try to detect if we are on macOS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # Check if a homebrew GCC is installed, otherwise fallback to Clang with libomp
    ifneq ($(shell command -v g++-14 2> /dev/null),)
        CXX = g++-14
        CXXFLAGS += -fopenmp
    else ifneq ($(shell command -v g++-13 2> /dev/null),)
        CXX = g++-13
        CXXFLAGS += -fopenmp
    else
        CXX = clang++
        CXXFLAGS += -Xpreprocessor -fopenmp -I$(shell brew --prefix libomp)/include
        LDFLAGS += -L$(shell brew --prefix libomp)/lib -lomp
    endif
else
    # Linux/Windows(MSYS2) default
    CXXFLAGS += -fopenmp
endif

TARGET = adaptive_sort
SRCS = main.cpp sorting.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.cpp sorting.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
