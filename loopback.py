# Rapsberry Pi: Loopback test script for ADC DAC pi board to test DAC and ADC chip readings / outputs
# Connect 10K resistor between output 1 and input 1
# Connect 10K resistor between output 2 and input 2
# run sudo python loopback.py

# Version 1.0  - 06/11/2013
# Version History:
# 1.0 - Initial Release
#
#

#!/usr/bin/python
import spidev
import time


DEBUG = 0
     
spi = spidev.SpiDev()
spi.open(0,0)
spi.max_speed_hz = (20000000)

spidac = spidev.SpiDev()
spidac.open(0,1)
spidac.max_speed_hz = (20000000)


counter = 1
# read SPI data from MCP3002 chip
def get_adc(channel):
	# Only 2 channels 0 and 1 else return -1
	if ((channel > 1) or (channel < 0)):
		return -1
	r = spi.xfer2([1,(2+channel)<<6,0])
        ret = ((r[1]&0x0F) << 8) + (r[2])
	return ret

def setOutput(channel, val):

        lowByte = val & 0xff;
        highByte = ((val >> 8) & 0xff) | channel << 7 | 0x1 << 5 | 1 << 4;
        spidac.xfer2([highByte, lowByte])
        
  
counter = 1
setOutput(0,0)
setOutput(1,0) 
setOutput(0,2048)
setOutput(1,4090) 
while(1):
	
	print "%02f %02f" % (get_adc(0),get_adc(1))
	#print get_adc(0)
	#print "Input 2: %02f" % get_adc(1)
	
	#counter = counter + 1