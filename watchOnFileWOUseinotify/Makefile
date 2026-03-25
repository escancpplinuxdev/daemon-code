CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = watchOnFileWOUseinotify

SRC =	watchOnFileWOUseinotify.cpp

OBJ = watchOnFileWOUseinotify.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)


$(OBJ) : $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC)



clean :
	rm -f $(OBJ) $(TARGET)


rebuild: clean all
