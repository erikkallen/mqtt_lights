#
# NewRemoteSwitch library v1.0.0 (20121229) made by Randy Simons http://randysimons.nl/
# See NewRemoteTransmitter.h for details.
#
# License: GPLv3. See license.txt
#
'use strict'
class Kaku
    constructor: (wpi, address, pin, periodusec, repeats) ->
        @address = address;
        @pin = pin;
        @periodusec = periodusec;
        @repeats = (1 << repeats) - 1 # I.e. _repeats = 2^repeats - 1
        @wpi = wpi#require('wiring-pi')
        @wpi.wiringPiSetup()

        @wpi.pinMode(@pin, @wpi.OUTPUT)

    sendGroup: (switchOn) =>
        for i in [0..@repeats]
            @sendStartPulse()
            @sendAddress()
            # Do send group bit
            @sendBit(true)

            # Switch on | off
            @sendBit(switchOn)

            # No unit. Is this actually ignored?..
            @sendUnitCode(0)

            @sendStopPulse()

    sendUnit: (unit, switchOn) =>
        for i in [0..@repeats]
            @sendStartPulse()
            @sendAddress()
            # Do send group bit
            @sendBit(false)

            # Switch on | off
            @sendBit(switchOn)

            @sendUnitCode(unit)

            @sendStopPulse()

    sendDim: (unit, dimLevel) =>
        for i in [0..@repeats]
            @sendStartPulse()
            @sendAddress()

            # No group bit
            @sendBit(false)

            #Switch type 'dim'
            @wpi.digitalWrite(@pin, @wpi.HIGH)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.LOW)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.HIGH)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.LOW)
            @wpi.delayMicroseconds(@periodusec)

            @sendUnitCode(unit);

            j = 3
            while j >= 0
                @sendBit dimLevel & 1 << j
                j--

            @sendStopPulse()

    sendStartPulse: =>
        @wpi.digitalWrite(@pin, @wpi.HIGH)
        @wpi.delayMicroseconds(@periodusec);
        @wpi.digitalWrite(@pin, @wpi.LOW);
        @wpi.delayMicroseconds(@periodusec * 10 + (@periodusec >> 1)) # Actually 10.5T insteat of 10.44T. Close enough.

    sendAddress: =>
        i = 25
        while i >= 0
            @sendBit((@address >> i) & 1);
            i--

    sendUnitCode: (unit) =>
        i = 3
        while i >= 0
            @sendBit(unit & 1<<i)
            i--

    sendStopPulse: =>
        @wpi.digitalWrite(@pin, @wpi.HIGH)
        @wpi.delayMicroseconds(@periodusec)
        @wpi.digitalWrite(@pin, @wpi.LOW)
        @wpi.delayMicroseconds(@periodusec * 40)

    sendBit: (isBitOne) =>
        if (isBitOne)
            # Send '1'
            @wpi.digitalWrite(@pin, @wpi.HIGH)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.LOW)
            @wpi.delayMicroseconds(@periodusec * 5)
            @wpi.digitalWrite(@pin, @wpi.HIGH)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.LOW)
            @wpi.delayMicroseconds(@periodusec)
        else
            #// Send '0'
            @wpi.digitalWrite(@pin, @wpi.HIGH)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.LOW)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.HIGH)
            @wpi.delayMicroseconds(@periodusec)
            @wpi.digitalWrite(@pin, @wpi.LOW)
            @wpi.delayMicroseconds(@periodusec * 5)

module.exports = Kaku
