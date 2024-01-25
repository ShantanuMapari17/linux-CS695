#!/bin/bash

make

echo "Running tests..."

insmod hello_proc.ko

echo "Module inserted"

rmmod hello_proc

echo "Module removed"

make clean