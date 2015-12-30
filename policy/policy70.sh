#!/bin/sh
IPTABLES=/sbin/iptables
IPSET=/sbin/ipset
MODPROBE=/sbin/modprobe

GEODIR="/etc/geo-blocking"
WWWPORT=8000

$IPTABLES -F -t nat
$IPTABLES -X
$IPTABLES -P INPUT DROP
$IPTABLES -P OUTPUT DROP
$IPTABLES -P FORWARD ACCEPT
$IPSET -N russia hash:net
$IPSET -N china hash:net

$MODPROBE ip_conntrack
$MODPROBE iptable_nat
$MODPROBE ip_conntrack_ftp
$MODPROBE ip_nat_ftp

# INPUT
# tracking rules
$IPTABLES -A INPUT -m state --state INVALID -j LOG --log-prefix "DROP INVALID " --log-ip-options --log-tcp-options
$IPTABLES -A INPUT -m state --state INVALID -j DROP
$IPTABLES -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# ACCEPT rules
$IPTABLES -A INPUT -p tcp --dport ${WWWPORT} --syn -m state --state NEW -j ACCEPT
$IPTABLES -A INPUT -p tcp --dport 22 --syn -m state --state NEW -j ACCEPT

# drop traffic from China and Russia
$IPSET flush china
$IPSET flush russia
for i in $(cat $GEODIR/cn.zone); do $IPSET -A china $i; done
for i in $(cat $GEODIR/ru.zone); do $IPSET -A russia $i; done
$IPTABLES -A INPUT -m set --match-set china src -j LOG --log-prefix "DROP CHINA " --log-level 6
$IPTABLES -A INPUT -m set --match-set china src -j DROP
$IPTABLES -A INPUT -m set --match-set russia src -j LOG --log-prefix "DROP CHINA " --log-level 6
$IPTABLES -A INPUT -m set --match-set russia src -j DROP

# default INPUT LOG rule
$IPTABLES -A INPUT -i !lo -j LOG --log-prefix "DROP " --log-ip-options --log-tcp-options

# prevent DDoS attacks
$IPTABLES -A INPUT -p tcp --dport ${WWWPORT} --syn -m limit --limit 30/minute --limit-burst 40 -j ACCEPT
$IPTABLES -A INPUT -p tcp --syn --dport 22 -m connlimit --connlimit-above 3 -j REJECT
$IPTABLES -A INPUT -p tcp --syn --dport ${WWWPORT} -m connlimit --connlimit-above 3 -j REJECT

# OUTPUT
# state tracking rules
$IPTABLES -A OUTPUT -m state --state INVALID -j LOG --log-prefix "DROP INVALID " --log-ip-options --log-tcp-options
$IPTABLES -A OUTPUT -m state --state INVALID -j DROP
$IPTABLES -A OUTPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

# ACCEPT rules for allowing connections out
$IPTABLES -A OUTPUT -p tcp --dport 22 -m state --state NEW -j ACCEPT
$IPTABLES -A OUTPUT -p tcp --dport 53 -m state --state NEW -j ACCEPT
$IPTABLES -A OUTPUT -p tcp --dport 80 -m state --state NEW -j ACCEPT
$IPTABLES -A OUTPUT -p tcp --dport 443 -m state --state NEW -j ACCEPT
$IPTABLES -A OUTPUT -p tcp --dport ${WWWPORT} -m state --state NEW,ESTABLISHED,RELATED -j ACCEPT
$IPTABLES -A OUTPUT -p udp --dport 53 -m state --state NEW -j ACCEPT
$IPTABLES -A OUTPUT -o !lo -j LOG --log-prefix "DROP " --log-ip-options --log-tcp-options

# FORWARD
# drop torrent
$IPTABLES -A FORWARD -m string --algo bm --string "BitTorrent" -j DROP
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "BitTorrent" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "BitTorrent protocol" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "peer_id=" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string ".torrent" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "announce.php?passkey=" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "torrent" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "announce" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "info_hash" -j DROP 
$IPTABLES -A FORWARD -p udp -m string --algo bm --string "tracker" -j DROP
$IPTABLES -A FORWARD -m string --string "get_peers" --algo bm -j DROP 
$IPTABLES -A FORWARD -m string --string "announce_peer" --algo bm -j LOGDROP 
$IPTABLES -A FORWARD -m string --string "find_node" --algo bm -j LOGDROP

# Trying forward
$IPTABLES -A FORWARD -p tcp --dport 6881:6889 -j DROP 
$IPTABLES -A FORWARD -p udp --dport 1024:65534 -j DROP
