OBJS = main.cpp network.o operation.o
CC = clang++
CFLAGS = -std=c++11 -Ofast -I /usr/include/eigen3/ -w -c
LFLAGS = -std=c++11 -Ofast -I /usr/include/eigen3/ -w

Build : $(OBJS)
	$(CC) $(LFLAGS) -o Build $(OBJS)

network.o : network/network.hpp network/network.cpp
	$(CC) $(CFLAGS) network/network.cpp -o network.o

operation.o : network/operation.hpp network/operation.cpp
	$(CC) $(CFLAGS) network/operation.cpp -o operation.o

clean:
	\rm *.o *~ build
