CCOMPILE      = $(CC) $(CFLAGS) $(INCLUDES) $(MACRO) $(OPTIONS)
CXXCOMPILE    = $(CXX) $(CPPFLAGS) $(INCLUDES) $(MACRO) $(OPTIONS)
CC            = gcc
CXX           = g++
CFLAGS       += -std=c17
CPPFLAGS     += -std=c++17
INCLUDES      = -I../src
MACRO         = -DSIMULATOR
OUTPUT_OPTION = -o $@

VPATH    =:../src/Driver
VPATH   +=:../src/MW:../src/MW/led
VPATH   +=:../src/App:../src/App/led:../src/App/network:../src/App/nvm:../src/App/task:../src/App/variant
VPATH   +=:mock

OBJECTS  = DeviceInterface
OBJECTS += MatrixLED LEDControllers
OBJECTS += AsciiMatrix JISMatrix Scroll
OBJECTS += StatusClient NVM
OBJECTS += NetworkWebServer NetworkSTAControl
OBJECTS += LEDControl SystemControl Variant
OBJECTS += NetworkMock NVMMock Max7219Mock
OBJECTS.o       = $(addsuffix .o  , $(OBJECTS))

.PHONY:	build
build:	simulator.out simulator.o $(OBJECTS.o)	# 中間ファイルを残す意図

.PHONY:	clean
clean:
	rm -f *.o *.out

%.out:	%.o $(OBJECTS.o)
	$(CXXCOMPILE) $(OUTPUT_OPTION) $^

%.o:	%.cpp
	$(CXXCOMPILE) -c $(OUTPUT_OPTION) $<

%.o:	%.c
	$(CCOMPILE) -c $(OUTPUT_OPTION) $<
