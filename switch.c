#include "switch.h"

void switches_print(struct switch_ctx ** switches, int num_switches){
    for (size_t i = 0; i < num_switches; i++) {
        printf("state_topic: %s\n", switches[i]->state_topic);
    }
}
