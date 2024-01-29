# Script for testing the spock program
#!/bin/bash

make

echo "[info]: Loading spock program"
insmod spock_driver.ko
echo "[info]: spock program loaded"

echo "[info]: Testing spock program"
# gcc -o app-test app-test.c
./app-test


echo "[info]: Unloading spock program"
rmmod spock_driver.ko

make clean