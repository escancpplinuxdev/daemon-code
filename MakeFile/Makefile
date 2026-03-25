CXX = g++
CXXFLAGS = -Wall

TARGET = calculator

OBJS = calculator.o log1.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

calculator.o: calculator.cpp log1.h
	$(CXX) $(CXXFLAGS) -c calculator.cpp

log1.o: log1.cpp log1.h
	$(CXX) $(CXXFLAGS) -c log1.cpp

clean:
	rm -f *.o $(TARGET)

rebuild:clean all
