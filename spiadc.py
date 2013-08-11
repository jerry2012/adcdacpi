#!/usr/bin/python
import spidev
import time
DEBUG = 0
     
spi = spidev.SpiDev()
spi.open(1,0)
     
# read SPI data from MCP3002 chip
def get_adc(channel):
	# Only 2 channels 0 and 1 else return -1
	if ((channel > 1) or (channel < 0)):
		return -1
	r = spi.xfer2([1,(2+channel)<<6,0])
           
	ret = ((r[1]&0x0F) << 8) + (r[2])
	return ret     

while(1):
	print get_adc(0)
	print get_adc(1)
	time.sleep(0.1)