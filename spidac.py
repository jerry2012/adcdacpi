#!/usr/bin/python
import spidev
import time
import RPi.GPIO as GPIO 
DEBUG = 0
     
spi = spidev.SpiDev()
spi.open(0,1)
spi.max_speed_hz = (4000000)
# set DAC voltage for MCP4822 12 bit dac

GPIO.setmode(GPIO.BOARD)
GPIO.setup(22, GPIO.OUT)
GPIO.output(22,GPIO.LOW)

def setOutput(channel, val):

	lowByte = val & 0xff;
	highByte = ((val >> 8) & 0xff) | channel << 7 | 0x1 << 5 | 1 << 4;
	spi.xfer2([highByte, lowByte])

while(1):
	for x in range(0, 4096,64):
		setOutput(0,x)
		setOutput(1,x)