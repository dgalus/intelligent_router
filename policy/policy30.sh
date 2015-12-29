#!/bin/sh
/bin/sh /opt/router/policy/static.sh

# syn-flood protection
$IPTABLES -A INPUT -p tcp ! --syn -m state --state NEW -j DROP

# drop null packets
$IPTABLES -A INPUT -p tcp --tcp-flags ALL NONE -j DROP

# drop invalid packets
$IPTABLES -A INPUT -m conntrack --ctstate INVALID -j DROP

# block range of ports (allow only ssh and gui panel)
$IPTABLES -A INPUT -m state --state NEW -p tcp --dport 1:21,23:7999,8001:65535 -j DROP
