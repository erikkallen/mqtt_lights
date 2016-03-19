#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <ctype.h>
#include <iostream>
//#include "RemoteSwitch.cpp"
#include "NewRemoteTransmitter.cpp"

using namespace std;

long map(long x, long in_min, long in_max, long out_min, long out_max);

int main(int argc, char **argv)
{
	int pin_out = 1; // Pin out using wiringPi pin numbering scheme (15 = TxD / BCM GPIO 14, see https://projects.drogon.net/raspberry-pi/wiringpi/pins/)
    int device = 0;
    char address = 'A';
    bool state = false;
    int brightness = 70;

    string statestr;


    if( argc < 4 || argc > 5 ) { // not enough arguments
    	std::cout << "usage: " << argv[0] << " address device state" << std::endl;
    	std::cout << "example: " << argv[0] << " 123 2 on" << std::endl;
	    return 0;
    } else {
	    address = atol(argv[1]);
	    device = atol(argv[2]);
	    statestr = argv[3];

        if (argc == 5) {
            brightness = atol(argv[4]);
            brightness = (int)map(brightness, 0,255,-1,15);
        }

	    if( statestr.compare("on") == 0 ) {
		    state = true;
	    } else {
		    state = false;
	    }
    }
    // load wiringPi
	if(wiringPiSetup() == -1)
	{
		printf("WiringPi setup failed. Maybe you haven't installed it yet?");
		exit(1);
	}

    // setup pin and make it low
	pinMode(pin_out, OUTPUT);
	digitalWrite(pin_out, LOW);
	//KaKuSwitch kaKuSwitch(pin_out);
    NewRemoteTransmitter transmitter(address, pin_out, 260, 3);
    if (argc == 5) {
        printf("Sending dim: addr: %d dev: %d state: %s brightness: %d\n", address,device,statestr.c_str(),brightness);

        transmitter.sendDim(device, brightness);
    } else {
        printf("Sending unit: addr: %d dev: %d state: %s brightness: %d\n", address,device,statestr.c_str(),brightness);

        transmitter.sendUnit(device, state);
    }

	//kaKuSwitch.sendSignal(address, device, state);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
