CPP_FILES := $(wildcard *.cpp)
OBJ_FILES := $(addprefix ./,$(notdir $(CPP_FILES:.cpp=.o)))
CXX     = g++
DEBUG   = -g
CXXFLAGS  = -Wall $(DEBUG) -std=c++14 -pthread
LDFLAGS = -pthread
NAME = reversi

all: $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(NAME) $^ $(LIBS)

clean:
	rm *.o
	rm $(NAME)
