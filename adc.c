// Rapsberry Pi: SPI in C to read MCP3202 SPI ADC Chip
// Compile:  gcc adc.c -std=c99 -o adc
// run ./dac

// Version 1.0  - 11/08/2013
// Version History:
// 1.0 - Initial Release
//
//
// Usage: GetVoltage(channel)
// channel = 0 or 1
// returns double
// Takes 100,000 samples and returns average and time elapsed
//
// Using MCP3202  - Output dual voltage 


#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <time.h>
#include <unistd.h>


#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define SPI_SPEED 2500000 // SPI frequency clock

static const char *device = "/dev/spidev0.0";
static uint8_t mode = SPI_MODE_0;		// SPI_MODE_0 

// used for timestamp code
struct timeval start, end;
long mtime, seconds, useconds;
// function variables
int dataA, dataB;
int fd;
int i;
static uint32_t speed = 2500000;


static void exit_on_error (const char *s)	// Exit and print error code
{ 	
	perror(s);
	abort();
} 
double GetVoltage(int channel) {
	double newval =0;
	int ret;
	double returnval;
	char  rx[3];
	uint16_t tx[] = {1,(2+channel)<<6,0,};


	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = 0,
		.speed_hz = speed,
		.bits_per_word = 8,
	};




	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);

	returnval = ((rx[1]&0x0F) << 8) + (rx[2]);
	// based on 3.3V supply
	return (3.3/4096)*returnval;


}
int main(int argc, char *argv[])
{
	// Open SPI device
	if ((fd = open(device, O_RDWR)) < 0) exit_on_error ("Can't open SPI device");

	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) exit_on_error ("Can't set Max Speed");
	// Set SPI mode
	if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) exit_on_error ("Can't set SPI mode");
	gettimeofday(&start, NULL);


	double average;
	int x =0;
	// loop to get average reading
	while ( x < 100000 ){
		average += GetVoltage(0);
		x++;
	}

	printf("%f\n", average/100000);
	gettimeofday(&end, NULL);

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;

	mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	printf("Elapsed time: %ld milliseconds\n", mtime);

	close(fd);
	return (0);
}