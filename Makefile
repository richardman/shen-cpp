CC = clang++
CFLAGS = -g -std=c++20

OBJS = GarbageCollection.o eval.o main.o parse.o primitives.o

.cpp.o:
	$(CC) $(CFLAGS) -c $<

shen-cpp: $(OBJS)
	$(CC) -g -o shen-cpp $(OBJS)	

GarbageCollection.o:	../../../cpp-gc/GarbageCollection.cpp
	$(CC) $(CFLAGS) -c $<
