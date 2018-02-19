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

check:
	./pmonodromy test/graphs/1.graph -t 1 -v -e Original -s 0 > test/out/1 2>&1
	./pmonodromy test/graphs/1.graph -t 1 -v -e Original -s 0 -old > test/out/1old 2>&1
	diff -I 'UseOldEVs' test/out/1 test/out/1old
	./pmonodromy test/graphs/1.graph -t 8 -v -e Original -s 0 > test/out/2 2>&1
	./pmonodromy test/graphs/1.graph -t 8 -v -e Original -s 0 -old > test/out/2old 2>&1
	diff -I 'UseOldEVs' test/out/2 test/out/2old
	./pmonodromy test/graphs/1.graph -t 8 -v -e WeightTowardCompleteNode -s 0 > test/out/3 2>&1
	./pmonodromy test/graphs/1.graph -t 8 -v -e WeightTowardCompleteNode -s 0 -old > test/out/3old 2>&1
	diff -I 'UseOldEVs' test/out/3 test/out/3old
	./pmonodromy test/graphs/2.graph -t 1 -v -e Original -s 0 > test/out/4 2>&1
	./pmonodromy test/graphs/2.graph -t 1 -v -e Original -s 0 -old > test/out/4old 2>&1
	diff -I 'UseOldEVs' test/out/4 test/out/4old
	./pmonodromy test/graphs/2.graph -t 8 -v -e Original -s 0 > test/out/5 2>&1
	./pmonodromy test/graphs/2.graph -t 8 -v -e Original -s 0 -old > test/out/5old 2>&1
	diff -I 'UseOldEVs' test/out/5 test/out/5old
	./pmonodromy test/graphs/2.graph -t 8 -v -e WeightTowardCompleteNode -s 0 > test/out/6 2>&1
	./pmonodromy test/graphs/2.graph -t 8 -v -e WeightTowardCompleteNode -s 0 -old > test/out/6old 2>&1
	diff -I 'UseOldEVs' test/out/6 test/out/6old
