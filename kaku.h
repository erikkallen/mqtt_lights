#ifndef KAKU_H
#define KAKU_H

#include <stdbool.h>

struct kaku_ctx {
    unsigned long address;
    unsigned short pin;
    unsigned int periodusec;
    unsigned short repeats;
};

void kaku_init(struct kaku_ctx * ctx, unsigned long address, unsigned short pin, unsigned int periodusec, unsigned short repeats);
void kaku_send_group(struct kaku_ctx * ctx, bool switchOn);
void kaku_send_unit(struct kaku_ctx * ctx, unsigned short unit, bool switchOn);
void kaku_send_dim(struct kaku_ctx * ctx, unsigned short unit, unsigned short dimLevel);

static void _sendStartPulse(struct kaku_ctx * ctx);
static void _sendAddress(struct kaku_ctx * ctx);
static void _sendUnit(struct kaku_ctx * ctx, unsigned short unit);
static void _sendStopPulse(struct kaku_ctx * ctx);
static void _sendBit(struct kaku_ctx * ctx, bool isBitOne);


#endif
