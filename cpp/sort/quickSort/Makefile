CXX = g++
CXXFLAGS = -Wall -Werror

TARGET = quickSort

#SRCS = quickSort.cpp 
#OBJS = $(SRCS:.cpp=.o)
OBJS = quickSort.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

#*.o: *.cpp *.h
#	$(CXX) $(CXXFLAGS) -c *.cpp

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)

# this command fire as --> make rebuild
rebuild:clean all

.PHONY: all clean rebuild
