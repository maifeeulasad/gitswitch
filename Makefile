CC = g++
CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -L/usr/local/include/rxcpp -pthread

gitswitch: gitswitch.cpp
	$(CC) -o gitswitch gitswitch.cpp $(CFLAGS) $(LIBS)
