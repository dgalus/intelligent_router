CC=g++
CFLAGS=-c -m64 -O2 -std=c++11 -Wall -fPIE -I.
SOCFLAGS=-Wall -g -fPIC -std=c++11 -shared
LIBS=-lpam -lpam_misc -ldl

all: www modules

www: helpers.o main.o server.o log.o module.o
	$(CC) -m64 -Wl,-O1 -o bin/www helpers.o main.o server.o log.o module.o $(LIBS)

helpers.o: src/helpers.cpp
	$(CC) $(CFLAGS) src/helpers.cpp

main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp

server.o: src/server.cpp
	$(CC) $(CFLAGS) src/server.cpp

log.o: src/log.cpp
	$(CC) $(CFLAGS) src/log.cpp

module.o: src/module.cpp
	$(CC) $(CFLAGS) src/module.cpp

modules: sites/src/css.cpp sites/src/firewall.cpp sites/src/index.cpp sites/src/interfaces.cpp sites/src/management.cpp sites/src/routing.cpp sites/src/interfaces_save.cpp
	rm helpers.o
	$(CC) -c -Wall -W -std=c++11 -fPIC -o helpers.o src/helpers.cpp
	$(CC) $(SOCFLAGS) -o sites/css.so sites/src/css.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/firewall.so sites/src/firewall.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/index.so sites/src/index.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/interfaces.so sites/src/interfaces.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/management.so sites/src/management.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/routing.so sites/src/routing.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/interfaces_save.so sites/src/interfaces_save.cpp helpers.o
	rm *.o

clean:
	rm *.o bin/www sites/*.so
