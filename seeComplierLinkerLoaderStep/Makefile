# ---------------- CONFIG ----------------
CXX = g++
CXXFLAGS = -g -Wall -Wextra -Iheader

SRC_DIR = source
OBJ_DIR = obj
BIN = a.out

# ---------------- FILES ----------------
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
# --------- wildcard -> find all .cpp files -> $(wildcard source/*.cpp) -> SRCS = source/a.cpp source/b.cpp source/c.cpp
# wildcard dynamically collects files matching a pattern

OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
# --------- patsubst -> convert .cpp -> .o  -> $(patsubst source/%.cpp, obj/%.o, $(SRCS))

# Pattern meaning
# source/%.cpp 	-> match any .cpp file
# obj/%.o	-> replace with .o file

# Expansion 
# source/a.cpp -> obj/a.o
# source/b.cpp -> obj/b.o
# source/c.cpp -> obj/c.o

# Final result:
# OBJS = obj/a.o obj/b.o obj/c.o

# patsubst transforms filenames using pattern substitution
# ---------------- BUILD RULE ----------------

all: $(BIN)
# default target is all
$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $(BIN)
# To build a.out → need all object files
# a.out: obj/a.o obj/b.o obj/c.o
#        g++ obj/a.o obj/b.o obj/c.o -o a.out

# Compile .cpp → .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@



#---------- $< symbol -> 	input file (source/a.cpp)
#---------- $@ symbol -> 	output file (obj/a.o)

# "To create obj/X.o, use source/X.cpp"
# @mkdir -p obj
# Create obj folder if not exists
# 
#	$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
#This is a pattern rule that tells make how to generate object files
#from source files automatically using pattern matching.
#
# ---------------- CLEAN ----------------
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# ---------------- RUN ----------------
run: all
	./$(BIN)
# ---------------- REBUILD --------------
rebuild: clean all

# ---------------- RERUN ----------------
rerun: clean run

#  What is $< and $@?
#$< → first dependency (input file)
#$@ → target file (output)


# why use patsubst?

#To automatically convert source filenames into object filenames
#without writing each manually.

#wildcard → finds files
#patsubst → converts filenames
#pattern rule → defines how to build files
#$<, $@ → automatic variables
