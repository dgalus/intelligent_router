#!/bin/sh
IPTABLES=/sbin/iptables

$IPTABLES -F -t nat
$IPTABLES -P INPUT ACCEPT
$IPTABLES -P OUTPUT ACCEPT
$IPTABLES -P FORWARD ACCEPT

$IPTABLES -F

/bin/sh /opt/router/policy/static.sh
