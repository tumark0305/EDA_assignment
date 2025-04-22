CC=g++
LDFLAGS=-std=c++11 -O3 -g -lm
SOURCES=*.cpp modules/*.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=legalizer
INCLUDES=*.h

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
