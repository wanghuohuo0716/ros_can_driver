#!/bin/bash
modprobe can_dev
modprobe can
modprobe can_raw
sudo ip link set can0 type can bitrate 250000
sudo ifconfig can0 up

