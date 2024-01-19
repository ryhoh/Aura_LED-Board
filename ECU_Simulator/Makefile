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
VPATH   +=:../src/MW
VPATH	+=:../src/MW/led
VPATH   +=:../src/App
VPATH	+=:../src/App/led
VPATH	+=:../src/App/network
VPATH	+=:../src/App/nvm
VPATH	+=:../src/App/task
VPATH	+=:../src/App/variant
VPATH   +=:../src/App/accelsensor
VPATH   +=:mock

OBJECTS  = DeviceInterface
OBJECTS += MatrixLED LEDControllers
OBJECTS += AsciiMatrix JISMatrix Scroll
OBJECTS += NVM
OBJECTS += AccelSensor Direction
OBJECTS += NetworkWebServer NetworkSTAControl
OBJECTS += LEDControl SystemControl Variant
OBJECTS += NetworkMock NVMMock Max7219Mock AccelSensorMock
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
