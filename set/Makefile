CC=g++
CFLAGS=-c -m64 -O2 -std=c++11 -Wall -fPIE -I.

all: set

set: set.o quagga_adapter.o adaptative_firewall_adapter.o
	$(CC) -m64 -Wl,-O1 set.o quagga_adapter.o adaptative_firewall_adapter.o -o bin/set -ldl
	rm *.o

set.o: src/set.cpp
	$(CC) $(CFLAGS) src/set.cpp

quagga_adapter.o: src/quagga_adapter.cpp
	$(CC) $(CFLAGS) src/quagga_adapter.cpp

adaptative_firewall_adapter.o: src/adaptative_firewall_adapter.cpp
	$(CC) $(CFLAGS) src/adaptative_firewall_adapter.cpp

clean:
	rm -rf *.o bin/set
