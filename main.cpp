#include "mqtt.h"
#include <stdio.h>
#include <yaml-cpp/yaml.h>

int main(int argc, char const *argv[]) {
    /* code */

    YAML::Node config = YAML::LoadFile("configuration.yaml");

    if (config["lastLogin"]) {
      std::cout << "Last logged in: " << config["lastLogin"].as<DateTime>() << "\n";
    }

    const std::string username = config["username"].as<std::string>();
    const std::string password = config["password"].as<std::string>();


    MQTT * mqttHdl;
    mqttHdl = new MQTT(NULL,"livingroom/light/switch/set", "10.0.0.21", 1883);

    while (1) {
        ;
    }
    return 0;
}
