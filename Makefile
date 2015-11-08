CC=g++
CFLAGS=-c -O2 -std=c++11 -Wall -fPIE -I.
SOCFLAGS=-Wall -g -fPIC -std=c++11 -shared
LIBS=-lpam -lpam_misc -ldl

all: criterions policies adaptativefirewall set show modules www
	rm *.o

adaptativefirewall: common.o log.o policyfactory.o criterionfactory.o configfile.o policyapplier.o helpers.o quagga_adapter.o adaptativefirewall.o
	$(CC) -Wl,-O1 common.o log.o policyfactory.o criterionfactory.o configfile.o policyapplier.o helpers.o quagga_adapter.o adaptativefirewall.o -o bin/adaptativefirewall -ldl

criterions: criterion/src/ping.cpp criterion/src/cpu_usage.cpp criterion/src/ram_usage.cpp criterion/src/processes.cpp
	$(CC) $(SOCFLAGS) -o criterion/ping.so criterion/src/ping.cpp
	$(CC) $(SOCFLAGS) -o criterion/cpu_usage.so criterion/src/cpu_usage.cpp
	$(CC) $(SOCFLAGS) -o criterion/ram_usage.so criterion/src/ram_usage.cpp
	$(CC) $(SOCFLAGS) -o criterion/processes.so criterion/src/processes.cpp
	$(CC) $(SOCFLAGS) -o criterion/interfaces_stats.so criterion/src/interfaces_stats.cpp

policies: policy/src/policy30.cpp
	$(CC) $(SOCFLAGS) -o policy/policy30.so policy/src/policy30.cpp

common.o: src/common.cpp
	$(CC) $(CFLAGS) src/common.cpp

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

set: set.o quagga_adapter.o adaptative_firewall_adapter.o helpers.o policyfactory.o common.o
	$(CC) -Wl,-O1 set.o quagga_adapter.o helpers.o adaptative_firewall_adapter.o policyfactory.o common.o -o bin/set -ldl

set.o: src/set.cpp
	$(CC) $(CFLAGS) src/set.cpp

show: show.o quagga_adapter.o adaptative_firewall_adapter.o helpers.o policyfactory.o common.o
	$(CC) -Wl,-O1 show.o quagga_adapter.o helpers.o adaptative_firewall_adapter.o policyfactory.o common.o -o bin/show -ldl

show.o: src/show.cpp
	$(CC) $(CFLAGS) src/show.cpp

quagga_adapter.o: src/quagga_adapter.cpp
	$(CC) $(CFLAGS) src/quagga_adapter.cpp

adaptative_firewall_adapter.o: src/adaptative_firewall_adapter.cpp
	$(CC) $(CFLAGS) src/adaptative_firewall_adapter.cpp

www: common.o policyfactory.o helpers.o main.o server.o log.o module.o quagga_adapter.o
	$(CC) -Wl,-O1 -o bin/www common.o policyfactory.o helpers.o main.o server.o log.o quagga_adapter.o module.o $(LIBS)

helpers.o: src/helpers.cpp
	$(CC) $(CFLAGS) src/helpers.cpp

main.o: src/main.cpp
	$(CC) $(CFLAGS) src/main.cpp

server.o: src/server.cpp
	$(CC) $(CFLAGS) src/server.cpp

module.o: src/module.cpp
	$(CC) $(CFLAGS) src/module.cpp

modules: sites/src/css.cpp sites/src/firewall.cpp sites/src/index.cpp sites/src/interfaces.cpp sites/src/management.cpp sites/src/routing.cpp sites/src/interfaces_save.cpp sites/src/firewall_save.cpp sites/src/management_save.cpp sites/src/routing_save.cpp src/helpers.cpp src/policyfactory.cpp src/common.cpp src/quagga_adapter.cpp
	rm helpers.o policyfactory.o
	$(CC) -c -Wall -W -std=c++11 -fPIC -o common.o src/common.cpp
	$(CC) -c -Wall -W -std=c++11 -fPIC -o policyfactory.o src/policyfactory.cpp
	$(CC) -c -Wall -W -std=c++11 -fPIC -o quagga_adapter.o src/quagga_adapter.cpp
	$(CC) -c -Wall -W -std=c++11 -fPIC -o helpers.o src/helpers.cpp
	$(CC) $(SOCFLAGS) -o sites/css.so sites/src/css.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/firewall.so sites/src/firewall.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/index.so sites/src/index.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/interfaces.so sites/src/interfaces.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/management.so sites/src/management.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/routing.so sites/src/routing.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/interfaces_save.so sites/src/interfaces_save.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/firewall_save.so sites/src/firewall_save.cpp helpers.o policyfactory.o common.o
	$(CC) $(SOCFLAGS) -o sites/management_save.so sites/src/management_save.cpp helpers.o
	$(CC) $(SOCFLAGS) -o sites/routing_save.so sites/src/routing_save.cpp helpers.o quagga_adapter.o
	$(CC) $(SOCFLAGS) -o sites/index_save.so sites/src/index_save.cpp helpers.o

clean:
	rm -rf *.o bin/adaptativefirewall bin/set bin/www sites/*.so bin/show criterion/*.so policy/*.so
