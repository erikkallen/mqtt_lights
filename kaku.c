/*
* NewRemoteSwitch library v1.0.0 (20121229) made by Randy Simons http://randysimons.nl/
* See NewRemoteTransmitter.h for details.
*
* License: GPLv3. See license.txt
*/

#include "kaku.h"
#include <wiringPi.h>


void kaku_init(struct kaku_ctx * ctx, unsigned long address, unsigned short pin, unsigned int periodusec, unsigned short repeats) {
    ctx->address = address;
    ctx->pin = pin;
    ctx->periodusec = periodusec;
    ctx->repeats = (1 << repeats) - 1; // I.e. _repeats = 2^repeats - 1

    pinMode(ctx->pin, OUTPUT);
}

void kaku_send_group(struct kaku_ctx * ctx, bool switchOn) {
    for (int i = ctx->repeats; i >= 0; i--) {
        _sendStartPulse(ctx);

        _sendAddress(ctx);

        // Do send group bit
        _sendBit(ctx,true);

        // Switch on | off
        _sendBit(ctx,switchOn);

        // No unit. Is this actually ignored?..
        _sendUnit(ctx,0);

        _sendStopPulse(ctx);
    }
}

void kaku_send_unit(struct kaku_ctx * ctx, unsigned short unit, bool switchOn) {
    for (int i = ctx->repeats; i >= 0; i--) {
        _sendStartPulse(ctx);

        _sendAddress(ctx);

        // No group bit
        _sendBit(ctx,false);

        // Switch on | off
        _sendBit(ctx,switchOn);

        _sendUnit(ctx,unit);

        _sendStopPulse(ctx);
    }
}

void kaku_send_dim(struct kaku_ctx * ctx, unsigned short unit, unsigned short dimLevel) {
    for (int i = ctx->repeats; i >= 0; i--) {
        _sendStartPulse(ctx);

        _sendAddress(ctx);

        // No group bit
        _sendBit(ctx,false);

        // Switch type 'dim'
        digitalWrite(ctx->pin, HIGH);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, LOW);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, HIGH);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, LOW);
        delayMicroseconds(ctx->periodusec);

        _sendUnit(ctx,unit);

        for (short j=3; j>=0; j--) {
            _sendBit(ctx,dimLevel & 1<<j);
        }

        _sendStopPulse(ctx);
    }
}

static void _sendStartPulse(struct kaku_ctx * ctx){
    digitalWrite(ctx->pin, HIGH);
    delayMicroseconds(ctx->periodusec);
    digitalWrite(ctx->pin, LOW);
    delayMicroseconds(ctx->periodusec * 10 + (ctx->periodusec >> 1)); // Actually 10.5T insteat of 10.44T. Close enough.
}

static void _sendAddress(struct kaku_ctx * ctx) {
    for (short i=25; i>=0; i--) {
        _sendBit(ctx,(ctx->address >> i) & 1);
    }
}

static void _sendUnit(struct kaku_ctx * ctx, unsigned short unit) {
    for (short i=3; i>=0; i--) {
        _sendBit(ctx,unit & 1<<i);
    }
}

static void _sendStopPulse(struct kaku_ctx * ctx) {
    digitalWrite(ctx->pin, HIGH);
    delayMicroseconds(ctx->periodusec);
    digitalWrite(ctx->pin, LOW);
    delayMicroseconds(ctx->periodusec * 40);
}

static void _sendBit(struct kaku_ctx * ctx, bool isBitOne) {
    if (isBitOne) {
        // Send '1'
        digitalWrite(ctx->pin, HIGH);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, LOW);
        delayMicroseconds(ctx->periodusec * 5);
        digitalWrite(ctx->pin, HIGH);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, LOW);
        delayMicroseconds(ctx->periodusec);
    } else {
        // Send '0'
        digitalWrite(ctx->pin, HIGH);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, LOW);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, HIGH);
        delayMicroseconds(ctx->periodusec);
        digitalWrite(ctx->pin, LOW);
        delayMicroseconds(ctx->periodusec * 5);
    }
}
