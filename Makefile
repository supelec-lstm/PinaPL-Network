OBJS = main.cpp network.o graph.o function.o
CC = clang++
CFLAGS = -std=c++11 -Ofast -I /usr/include/eigen3/ -w -c
LFLAGS = -std=c++11 -Ofast -I /usr/include/eigen3/ -w

Build : $(OBJS)
	$(CC) $(LFLAGS) -o Build $(OBJS)

network.o : network/network.cpp graph.o
	$(CC) $(CFLAGS) network/network.cpp -o network.o

graph.o : network/graph.cpp
	$(CC) $(CFLAGS) network/graph.cpp -o graph.o

function.o : network/function.cpp
	$(CC) $(CFLAGS) network/function.cpp -o function.o

clean:
	rm *.o *~

mrproper:
	rm Build