#ifndef SWITCH_H
#define SWITCH_H

#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

struct switch_ctx {
    uint8_t address;
    uint8_t device;
    char state_topic[255];
    char cmd_topic[255];
    char brightness_topic[255];
    char brightness_state_topic[255];
};

void switch_init(struct switch_ctx * ctx,
                uint8_t address,
                uint8_t device,
                char * state_topic,
                char * cmd_topic,
                char * brightness_topic,
                char * brightness_state_topic);

void switches_print(struct switch_ctx * switches,int num_switches);

#endif
