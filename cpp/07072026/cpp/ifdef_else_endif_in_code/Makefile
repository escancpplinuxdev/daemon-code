# make rebuild && ./ifdef_else_endif_in_code
# rm -f *.o ifdef_else_endif_in_code
# g++ -Wall -Iheader -o ifdef_else_endif_in_code source/ifdef_else_endif_in_code.cpp source/print.cpp -D__WMCBUILD__

CXX= g++

CXXFLAGS= -Wall -Iheader

BUILDFLAGS=$(shell echo `cat /tmpdata/cpp/07072026/cpp/ifdef_else_endif_in_code/buildflags 2>/dev/null`)

TARGET= ifdef_else_endif_in_code

OBJS= source/ifdef_else_endif_in_code.cpp source/print.cpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(BUILDFLAGS)
	@echo $(TARGET) done .

clean :
	rm -f *.o $(TARGET)

rebuild : clean all

.PHONY: all clean rebuild
