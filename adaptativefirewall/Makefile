CC=g++
CFLAGS=-c -m64 -O2 -std=c++11 -Wall -fPIE -I.
SOCFLAGS=-Wall -g -fPIC -std=c++11 -shared

all: adaptativefirewall criterions policies

adaptativefirewall: filewriter.o log.o policyfactory.o criterionfactory.o configfile.o policyapplier.o adaptativefirewall.o
	$(CC) -m64 -Wl,-O1 filewriter.o log.o policyfactory.o criterionfactory.o configfile.o policyapplier.o adaptativefirewall.o -o bin/adaptativefirewall -ldl
	rm *.o

criterions: criterion/src/ping.cpp criterion/src/cpu_usage.cpp criterion/src/ram_usage.cpp criterion/src/processes.cpp
	$(CC) $(SOCFLAGS) -o criterion/ping.so criterion/src/ping.cpp
	$(CC) $(SOCFLAGS) -o criterion/cpu_usage.so criterion/src/cpu_usage.cpp
	$(CC) $(SOCFLAGS) -o criterion/ram_usage.so criterion/src/ram_usage.cpp
	$(CC) $(SOCFLAGS) -o criterion/processes.so criterion/src/processes.cpp
	$(CC) $(SOCFLAGS) -o criterion/interaces_stats.so criterion/src/interfaces_stats.cpp

policies: policy/src/policy30.cpp
	$(CC) $(SOCFLAGS) -o policy/policy30.so policy/src/policy30.cpp


filewriter.o: src/filewriter.cpp
	$(CC) $(CFLAGS) src/filewriter.cpp

log.o: src/log.cpp
	$(CC) $(CFLAGS) src/log.cpp

policyfactory.o: src/policyfactory.cpp
	$(CC) $(CFLAGS) src/policyfactory.cpp

criterionfactory.o: src/criterionfactory.cpp
	$(CC) $(CFLAGS) src/criterionfactory.cpp

configfile.o: src/configfile.cpp
	$(CC) $(CFLAGS) src/configfile.cpp

policyapplier.o: src/policyapplier.cpp
	$(CC) $(CFLAGS) src/policyapplier.cpp

adaptativefirewall.o: src/adaptativefirewall.cpp
	$(CC) $(CFLAGS) src/adaptativefirewall.cpp

clean:
	rm -rf *.o bin/adaptativefirewall criterion/*.so policy/*.so
