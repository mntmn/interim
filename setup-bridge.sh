#!/bin/bash
brctl addbr br0
if command -v ifconfig >/dev/null 2>&1; then
  echo "Using ifconfig"
  ifconfig br0 10.0.0.1
elif command -v ip >/dev/null 2>&1; then
  echo "Using ip"
  ip link set br0 up
  ip addr add 10.0.0.1 broadcast 10.0.0.255 dev br0
else
  echo "I don\'t now how to configurate the bridge on your machine"
fi;
