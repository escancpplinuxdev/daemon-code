CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -Iheader
LDFLAGS = -lcrypto

TARGETS = encrypt decrypt

all: $(TARGETS)

encrypt: source/encrypt.cpp header/encrypt.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

decrypt: source/decrypt.cpp header/decrypt.h
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(TARGETS)

rebuild: clean all

.PHONY: all clean rebuild
