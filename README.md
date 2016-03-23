# MQTT Service for Klik aan Klik uit and more (in the future)

I use this application together with [Home-Assistant](home-assistant.io) to control my lights

It is designed to be run on a Raspberry Pi

### Dependencies

* WiringPi (sudo apt-get install wiringpi)
* libmosquitto (sudo apt-get install libmosquitto-dev)
* libmosquittopp (sudo apt-get install libmosquittopp-dev)

Most of the code for controlling the radio I found on http://weejewel.tweakblogs.net/blog/8665/lampen-schakelen-met-een-raspberry-pi
Signal and slot code from https://github.com/pbhogan/Signals by Patrick Hogan
