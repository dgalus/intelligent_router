#!/bin/sh
/bin/sh /opt/router/policy/policy30.sh

# block China and Russia traffic
/bin/sh /opt/router/policy/geoblock/block_russia.sh
/bin/sh /opt/router/policy/geoblock/block_china.sh

# prevent DDoS attacks
$IPTABLES -A INPUT -m limit --limit 300/minute --limit-burst 300 -j ACCEPT

# limit connections on ssh
$IPTABLES -A INPUT -p tcp --syn --dport 22 -m connlimit --connlimit-above 3 -j REJECT

# limit connections on www
$IPTABLES -A INPUT -p tcp --syn --dport ${WWWPORT} -m connlimit --connlimit-above 3 -j REJECT

# block ping
$IPTABLES -A INPUT -p icmp --icmp-type echo-reply -s 0/0 -j ACCEPT
$IPTABLES -A INPUT -p icmp --icmp-type destination-unreachable -s 0/0 -j ACCEPT
$IPTABLES -A INPUT -p icmp --icmp-type time-exceeded -s 0/0 -j ACCEPT
$IPTABLES -A INPUT -p icmp -j DROP
