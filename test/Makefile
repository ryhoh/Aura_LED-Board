CCOMPILE      = $(CC) $(CFLAGS) $(INCLUDES) $(MACRO) $(OPTIONS)
CXXCOMPILE    = $(CXX) $(CPPFLAGS) $(INCLUDES) $(MACRO) $(OPTIONS)
CC            = gcc
CXX           = g++
CFLAGS       += -std=c17
CPPFLAGS     += -std=c++17
INCLUDES      = -I../src
OUTPUT_OPTION = -o $@

TESTLINK = -pthread -lgtest_main -lgtest
VPATH    = ../src

TARGETS = MatrixLED AsciiMatrix JISMatrix
OBJECTS = MatrixLED AsciiMatrix JISMatrix

testTARGETS     = $(addprefix test, $(TARGETS))
testTARGETS.out = $(addsuffix .out, $(testTARGETS))
OBJECTS.o       = $(addsuffix .o  , $(OBJECTS))

.PHONY:	test
test:	build clean;
.PHONY:	build
build:	$(testTARGETS.out)

.PHONY:	clean
clean:
	rm -f *.o *.out

%.out:	%.o $(OBJECTS.o)
	$(CXXCOMPILE) $(OUTPUT_OPTION) $(TESTLINK) $^
	./$@

%.o:	%.cpp
	$(CXXCOMPILE) -c $(OUTPUT_OPTION) $<

%.o:	%.c
	$(CCOMPILE) -c $(OUTPUT_OPTION) $<
