#!/bin/bash

echo "remap the device serial port(ttyUSBX) to arduino"
echo "arduino usb connection as /dev/arduino, check it using the command : ls -l /dev | grep ttyUSB"
echo "start copy arduino.rules to /etc/udev/rules.d/"
source /usr/share/colcon_cd/function/colcon_cd.sh
colcon_cd diffdrive_arduino
sudo cp scripts/arduino.rules  /etc/udev/rules.d/
echo -e "\nRestarting udev\n"
sudo service udev reload
sudo service udev restart
sudo udevadm control --reload && sudo udevadm trigger
echo "finish"
