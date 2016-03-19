#include "mqtt.h"
#include <string>
#include <wiringPi.h>

MQTT::MQTT(const char * _id,const char * _topic, const char * _host, int _port) : mosquittopp(_id)
{
    mosqpp::lib_init();        // Mandatory initialization for mosquitto library
    this->keepalive = 60;    // Basic configuration setup for myMosq class
    this->id = _id;
    this->port = _port;
    this->host = _host;
    this->topic = _topic;
    if(wiringPiSetup() == -1)
    {
        std::cout << "WiringPi setup failed. Maybe you haven't installed it yet?";
        exit(1);
    }
    pinMode(1, OUTPUT);
    digitalWrite(1, LOW);
    this->transmitter = new NewRemoteTransmitter(123, 1, 260, 3);
    this->transmitter->sendUnit(2, true); // initial state
    this->transmitter->sendDim(2, 15);
    connect_async(host, port, keepalive);
    loop_start();            // Start thread managing connection / publish / subscribe
};

MQTT::~MQTT() {
    loop_stop();            // Kill the thread
    mosqpp::lib_cleanup();    // Mosquitto library cleanup
}

bool MQTT::send_message(const  char * _topic, const  char * _message)
{
    // Send message - depending on QoS, mosquitto lib managed re-submission this the thread
    //
    // * NULL : Message Id (int *) this allow to latter get status of each message
    // * topic : topic to be used
    // * lenght of the message
    // * message
    // * qos (0,1,2)
    // * retain (boolean) - indicates if message is retained on broker or not
    // Should return MOSQ_ERR_SUCCESS
    int ret = publish(NULL,_topic,strlen(_message),_message,1,true);
    return ( ret == MOSQ_ERR_SUCCESS );
}
void MQTT::on_disconnect(int rc) {
    std::cout << ">> myMosq - disconnection(" << rc << ")" << std::endl;
}


void MQTT::on_connect(int rc)
{
    if ( rc == 0 ) {
        std::cout << ">> myMosq - connected with server" << std::endl;
        subscribe(NULL, this->topic);
        subscribe(NULL, "livingroom/light/brightness/set");
        send_message("livingroom/light/switch/state","on");
        send_message("livingroom/light/brightness/state","255");
    } else {
        std::cout << ">> myMosq - Impossible to connect with server(" << rc << ")" << std::endl;
    }
}


void MQTT::on_publish(int mid)
{
    std::cout << ">> myMosq - Message (" << mid << ") succeed to be published " << std::endl;
}

void MQTT::set_state(int new_state)
{
    if (this->state != new_state) {
        this->state = new_state;
        on_state_updated(new_state);

    }

}

void MQTT::on_state_updated(int new_state)
{
    std::cout << ">> myMosq - State updated (" << new_state << ") " << std::endl;
    this->transmitter->sendUnit(2, new_state);
}

void MQTT::on_message(const struct mosquitto_message *message) {
    std::string msg((char*)message->payload) ;
    std::string topic((char*)message->topic) ;
    std::cout << ">> myMosq - Message (" << msg << ") received in topic " << topic << std::endl;
    if (msg.compare("on")==0) {
        set_state(1);
        send_message("livingroom/light/switch/state","on");
    } else if (msg.compare("off")==0) {
        set_state(0);
        send_message("livingroom/light/switch/state","off");
    } else {

        int brightness = std::stoi(msg);
        brightness = (int)map(brightness, 0,255,1,15);
        std::cout << ">> myMosq - Set brightness (" << brightness << ") " << std::endl;

        this->transmitter->sendDim(2, brightness);
    }

}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
