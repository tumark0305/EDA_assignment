CC=g++
LDFLAGS=-std=c++11 -O3 -g -lm
SOURCES=*.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=genPlot
INCLUDES=*.h

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

%.o:  %.c  ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE)
