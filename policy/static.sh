#!/bin/sh

IPTABLES=/sbin/iptables
IPSET=/sbin/ipset
WWWPORT=8000

$IPTABLES -F

# allow output connections
$IPTABLES -A OUTPUT -j ACCEPT

# allow forwarding
$IPTABLES -A FORWARD -j ACCEPT
