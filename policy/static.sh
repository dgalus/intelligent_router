#!/bin/sh

IPTABLES=/sbin/iptables
IPSET=/sbin/ipset
WWWPORT=8000

$IPTABLES -F

# prevent DDoS attacks
$IPTABLES -A INPUT -m limit --limit 50/minute --limit-burst 200 -j ACCEPT

# syn-flood protection
$IPTABLES -A INPUT -p tcp ! --syn -m state --state NEW -j DROP

# drop null packets
$IPTABLES -A INPUT -p tcp --tcp-flags ALL NONE -j DROP

# limit connections on ssh
$IPTABLES -A INPUT -p tcp --syn --dport 22 -m connlimit --connlimit-above 3 -j REJECT

# limit connections on www
$IPTABLES -A INPUT -p tcp --syn --dport ${WWWPORT} -m connlimit --connlimit-above 3 -j REJECT

# block China and Russia traffic
/bin/sh /opt/router/policy/geoblock/block_russia.sh
/bin/sh /opt/router/policy/geoblock/block_china.sh

# allow output connections
$IPTABLES -A OUTPUT -j ACCEPT

# allow forwarding
$IPTABLES -A FORWARD -j ACCEPT
