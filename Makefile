CC=g++
CFLAGS=-c -Wall -g -O0
LDFLAGS=-lrt
SOURCES=family.cpp date_class.cpp people.cpp person.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Family

all: $(SOURCES) $(EXECUTABLE)
		
$(EXECUTABLE): $(OBJECTS) 
		$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
	-rm $(EXECUTABLE) $(OBJECTS)

