#!/bin/sh
GEODIR="/etc/geo-blocking"
COUNTRYZONE="ru.zone"
COUNTRYNAME="russia"
IPSET=/sbin/ipset
IPTABLES=/sbin/iptables

$IPSET -N $COUNTRYNAME hash:net
rm -f $GEODIR/$COUNTRYZONE

$IPSET flush $COUNTRYNAME

wget -P $GEODIR http://www.ipdeny.com/ipblocks/data/countries/$COUNTRYZONE

for i in $(cat $GEODIR/$COUNTRYZONE); do $IPSET -A $COUNTRYNAME $i; done

$IPTABLES -A INPUT -m set --match-set $COUNTRYNAME src -j LOG --log-prefix "iptables: DROP Russia" --log-level 6
$IPTABLES -A INPUT -m set --match-set $COUNTRYNAME src -j DROP
