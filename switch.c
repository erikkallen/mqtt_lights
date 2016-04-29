#include "switch.h"


void switch_init(struct switch_ctx * ctx,
                uint8_t address,
                uint8_t device,
                char * state_topic,
                char * cmd_topic,
                char * brightness_topic,
                char * brightness_state_topic) {

}

void switches_print(struct switch_ctx * switches,int num_switches) {
    for (size_t i = 0; i < num_switches; i++) {
        /* code */
        printf("%s\n", switches[i].state_topic);
    }
}
