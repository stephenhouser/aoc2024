# Makefile - generic makefile template for /r/dailyprogrammer
#
# Replace TEMPLATE with the name of the target.
#
# 2017-02-10/Stephen Houser
#
DAY = $(shell basename $$PWD)
TARGET = solution
HEADERS =
SOURCES = $(TARGET).cpp

# Gnu make specific!
ifdef $(SOURCES)
# Include only .c files from $(SOURCES)
C_SOURCES = $(filter %.c,$(SOURCES))
CXX_SOURCES = $(filter %.cpp,$(SOURCES))
OBJECTS = $(C_SOURCES:.c=.o) $(CXX_SOURCES:.cpp=.o)
else
# Include all .c files in current directory
SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(patsubst %.cpp, %.o, $(wildcard *.cpp)) \
			$(patsubst %.c, %.o, $(wildcard *.c))
endif

# C Preprocessor flags (for c and c++ code)
#CPPFLAGS = -g -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror -Wpedantic
CPPFLAGS = -g -Wall -Weffc++ -Wextra -Wconversion -Werror -Wpedantic

# C specific flags
CC = gcc
CFLAGS =
LFLAGS =

# C++ specific flags
CXX = g++
CXXFLAGS = -std=c++17 -O3
LXXFLAGS =

.PHONY: default all clean distclean

default: $(TARGET)

all: default


info:
	@echo SOURCES= $(SOURCES)
	@echo HEADERS= $(HEADERS)
	@echo OBJECTS= $(OBJECTS)

# default rule for compiling c code
#%.o: %.c $(HEADERS)
#	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

# default rule for compiling c++ code
#%.o: %.cpp $(HEADERS)
#	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

# https://www.gnu.org/software/make/manual/html_node/Special-Targets.html
# if make is killed or interrupted during the execution of their
# recipes, the target is not deleted.
.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CXX) $(LXXFLAGS) $^ $(LIBS) -o $@

lint: $(SOURCES)
	clang-tidy $(SOURCES)

test: $(TARGET)
	@-/bin/echo -n "$(DAY) "
	@-./$(TARGET) -v -s 7036 -g 2 test.txt || true
	@-/bin/echo -n "$(DAY) "
	@-./$(TARGET) -v -s 11048 -g 2 test-2.txt || true
	@-/bin/echo -n "$(DAY) "
	@-./$(TARGET) -v -s 83444 -g 2 input.txt || true

clean:
	-rm -f *.o
	-rm -f core a.out

distclean: clean
	-rm -f $(TARGET)
