CC = g++
CFLAGS = -std=c++11
OBJ = homotopy_graph_types.o our_strategy.o testing.o main.o

all: pmonodromy

pmonodromy: $(OBJ)
	$(CC) $(CFLAGS) -o pmonodromy $(OBJ)
    
homotopy_graph_types.o: homotopy_graph_types.cpp homotopy_graph_types.h
	$(CC) $(CFLAGS) -c homotopy_graph_types.cpp 
    
our_strategy.o: our_strategy.cpp our_strategy.h
	$(CC) $(CFLAGS) -c our_strategy.cpp
    
testing.o: testing.cpp testing.h
	$(CC) $(CFLAGS) -c testing.cpp
    
main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

clean:
	/bin/rm -f *.o pmonodromy
