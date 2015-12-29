#!/bin/sh

IPTABLES=/sbin/iptables
IPSET=/sbin/ipset
WWWPORT=8000

$IPTABLES -F

# allow output connections
$IPTABLES -A OUTPUT -j ACCEPT

# allow forwarding
$IPTABLES -A FORWARD -j ACCEPT

# limit connections per IP
$IPTABLES -A INPUT -p tcp --syn -m connlimit --connlimit-above 5 -j REJECT
