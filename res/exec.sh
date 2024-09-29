#!/bin/bash

xset -dpms
xset s off
openbox-session &
start-pulseaudio-x11

while true; do
	cd /var/samanpark
	/var/samanpark/SamanParkKiosk
done
