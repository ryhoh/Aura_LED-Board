CCOMPILE      = $(CC) $(CFLAGS) $(INCLUDES) $(MACRO) $(OPTIONS)
CXXCOMPILE    = $(CXX) $(CPPFLAGS) $(INCLUDES) $(MACRO) $(OPTIONS)
CC            = gcc
CXX           = g++
CFLAGS       += -std=c17
CPPFLAGS     += -std=c++17
INCLUDES      = -I../src
MACRO         = -DSIMULATOR
OUTPUT_OPTION = -o $@

VPATH    = ../src

OBJECTS = MatrixLED AsciiMatrix JISMatrix
OBJECTS.o       = $(addsuffix .o  , $(OBJECTS))

.PHONY:	test
test:	simulator.out
	./simulator.out

.PHONY:	clean
clean:
	rm -f *.o *.out

%.out:	%.o $(OBJECTS.o)
	$(CXXCOMPILE) $(OUTPUT_OPTION) $(TESTLINK) $^

%.o:	%.cpp
	$(CXXCOMPILE) -c $(OUTPUT_OPTION) $<

%.o:	%.c
	$(CCOMPILE) -c $(OUTPUT_OPTION) $<
