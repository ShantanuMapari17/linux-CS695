#!/bin/bash

## Building the applications
make 

## Build your ioctl driver and load it here



###############################################

# Launching the control station
./control_station &
c_pid=$!
echo "Control station PID: $c_pid"

# Launching the soldier
./soldier $c_pid &
echo "Soldier PID: $!"

sleep 2
kill -9 $c_pid

## Remove the driver here


