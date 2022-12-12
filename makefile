CC=g++
#  -g		- adds debugging
#  -Wall	- turn on most compiler warnings
FLAGS=-g -Wall 
LIBS=-pthread -lrt

TARGET=RayTracer
SOURCE=./RayTracer/
DEST=./x64/Release/

all: 
	$(CC) -o $(DEST)$(TARGET) $(SOURCE)$(TARGET).cpp $(SOURCE)ppm.cpp

debug:
	$(CC) $(FLAGS) -o $(DEST)$(TARGET) $(SOURCE)$(TARGET).cpp $(SOURCE)ppm.cpp

clean:
	rm -rf *.o $(TARGET)
