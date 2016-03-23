#include "mqtt.h"
#include <stdio.h>
#include "json.hpp"

// for convenience
using json = nlohmann::json;

int main(int argc, char const *argv[]) {
    /* code */

    MQTT * mqttHdl;
    mqttHdl = new MQTT(NULL,"livingroom/light/switch/set", "10.0.0.21", 1883);

    while (1) {
        ;
    }
    return 0;
}
