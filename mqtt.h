#include <mosquittopp.h>
#include <iostream>
#include <string.h>
#include "NewRemoteTransmitter.h"
#include "json.hpp"

// for convenience
using json = nlohmann::json;

class MQTT : public mosqpp::mosquittopp
{
private:
    const char     *     host;
    const char    *     id;
    const char    *     topic;
    NewRemoteTransmitter * transmitter;
    int                port;
    int                keepalive;
    int state;

    void on_connect(int rc);
    void on_disconnect(int rc);
    void on_publish(int mid);
    void on_message(const struct mosquitto_message *message);

public:
    MQTT(const char *id, const char * _topic, const char *host, int port);
    ~MQTT();
    bool send_message(const char * _topic, const char * _message);
    void set_state(int new_state);
    void on_state_updated(int new_state);
};

long map(long x, long in_min, long in_max, long out_min, long out_max);
