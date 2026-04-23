CXX = g++
CXXFLAGS = -Wall -Werror

TARGET = mergeSort

#SRCS = quickSort.cpp  a.cpp 						# if you have multiple .cpp files and header file 
#OBJS = $(SRCS:.cpp=.o)
OBJS = mergeSort.o
#OBJS = *.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

#*.o: *.cpp *.h
#	$(CXX) $(CXXFLAGS) -c *.cpp

%.o	 : %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean	 :
	rm -f *.o $(TARGET)

rebuild	 :clean all 								# this command fire as --> make rebuild

.PHONY	 : all clean rebuild
