OBJS = main.cpp network.o graph.o
CC = clang++
CFLAGS = -std=c++11 -Ofast -I /usr/include/eigen3/ -w -c
LFLAGS = -std=c++11 -Ofast -I /usr/include/eigen3/ -w

Build : $(OBJS)
	$(CC) $(LFLAGS) -o Build $(OBJS)

network.o : network/network.cpp node.o graph.o
	$(CC) $(CFLAGS) network/network.cpp -o network.o

graph.o : network/graph.cpp
	$(CC) $(CFLAGS) network/graph.cpp -o graph.o

clean:
	rm *.o *~

mrproper:
	rm Build