CC=g++
CFLAGS=-c -m64 -O2 -std=c++11 -Wall -fPIE -I.

all: show

show: show.o quagga_adapter.o adaptative_firewall_adapter.o
	$(CC) -m64 -Wl,-O1 show.o quagga_adapter.o adaptative_firewall_adapter.o -o bin/show -ldl
	rm *.o

show.o: src/show.cpp
	$(CC) $(CFLAGS) src/show.cpp

quagga_adapter.o: src/quagga_adapter.cpp
	$(CC) $(CFLAGS) src/quagga_adapter.cpp

adaptative_firewall_adapter.o: src/adaptative_firewall_adapter.cpp
	$(CC) $(CFLAGS) src/adaptative_firewall_adapter.cpp

clean:
	rm -rf *.o bin/show
