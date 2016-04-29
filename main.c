#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <json.h>
#include "switch.h"

void json_parse(json_object * jobj,struct switch_ctx ** switches, int * num_switches);
void print_json_value(json_object *jobj);
void json_parse_array( json_object *jobj, char *key,struct switch_ctx ** switches, int * num_switches);
void json_value_parse(json_object * jobj,struct switch_ctx * switches);
/*
#include <mosquitto.h>
#include "kaku.h"

#define mqtt_host "10.0.0.21"
#define mqtt_port 1883
#define KAKU_ADDR 123
#define KAKU_DEV  2
#define KAKU_PIN  1

static int run = 1;

static struct kaku_ctx kctx;
struct switch_ctx sctx[2];


void handle_signal(int s)
{
run = 0;
}

void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
bool match = 0;
printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);

mosquitto_topic_matches_sub("/test", message->topic, &match);
if (match) {
printf("got message for ADC topic\n");
}

}
*/
int main(int argc, char *argv[])
{
    FILE * config_fp;
    config_fp = fopen("config.json","rb");

    fseek(config_fp, 0, SEEK_END);
    long fsize = ftell(config_fp);
    fseek(config_fp, 0, SEEK_SET);

    char *config_j = malloc(fsize + 1);
    fread(config_j, fsize, 1, config_fp);
    fclose(config_fp);

    config_j[fsize] = 0;

    json_object * jobj = json_tokener_parse(config_j);

    struct switch_ctx * switches;
    int num_switches;
    json_parse(jobj, &switches, &num_switches);

    printf("After parse %p\n", switches);

    switches_print(switches,num_switches);

    //free(switches);
    /* kaku_init(kctx, KAKU_ADDR,KAKU_PIN,260,3);
    uint8_t reconnect = true;
    char clientid[24];
    struct mosquitto *mosq;
    int rc = 0;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    mosquitto_lib_init();

    memset(clientid, 0, 24);
    snprintf(clientid, 23, "mysql_log_%d", getpid());
    mosq = mosquitto_new(clientid, true, 0);

    if(mosq){
    mosquitto_connect_callback_set(mosq, connect_callback);
    mosquitto_message_callback_set(mosq, message_callback);

    rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);

    mosquitto_subscribe(mosq, NULL, "/test", 0);

    while(run){
    rc = mosquitto_loop(mosq, -1, 1);
    if(run && rc){
    printf("connection error!\n");
    sleep(10);
    mosquitto_reconnect(mosq);
}
}
mosquitto_destroy(mosq);
}

mosquitto_lib_cleanup();

return rc;*/

return 0;
}


/*printing the value corresponding to boolean, double, integer and strings*/
void print_json_value(json_object *jobj){
    enum json_type type;
    printf("type: ");
    type = json_object_get_type(jobj); /*Getting the type of the json object*/
    switch (type) {
        case json_type_boolean: printf("json_type_boolean\n");
        printf("value: %s\n", json_object_get_boolean(jobj)? "true": "false");
        break;
        case json_type_double: printf("json_type_double\n");
        printf("          value: %lf\n", json_object_get_double(jobj));
        break;
        case json_type_int: printf("json_type_int\n");
        printf("          value: %d\n", json_object_get_int(jobj));
        break;
        case json_type_string: printf("json_type_string\n");
        printf("          value: %s\n", json_object_get_string(jobj));
        break;
        case json_type_null:
        printf("          value: null\n");
        break;
    }

}

void json_parse_array( json_object *jobj, char *key,struct switch_ctx ** switches, int * num_switches) {
    //void json_parse(json_object * jobj,struct switch_ctx * switches); /*Forward Declaration*/
    enum json_type type;

    json_object *jarray = jobj; /*Simply get the array*/
    if(key) {
        json_object_object_get_ex(jobj, key, &jarray); /*Getting the array if it is a key value pair*/
    }

    int arraylen = json_object_array_length(jarray); /*Getting the length of the array*/
    printf("Array Length: %d\n",arraylen);

    printf("Malloc switch array\n");
    *switches = (struct switch_ctx *)malloc(sizeof(struct switch_ctx*) * arraylen);
    *num_switches = arraylen;

    printf("%p\n", switches);

    int i;
    json_object * jvalue;

    for (i=0; i< arraylen; i++){
        jvalue = json_object_array_get_idx(jarray, i); /*Getting the array element at position i*/
        type = json_object_get_type(jvalue);
        if (type == json_type_object) {
            printf("Found light object\n");
            json_value_parse(jvalue,switches[i]);
        }
    }
}

void json_value_parse(json_object * jobj,struct switch_ctx * switches) {
    enum json_type type;
    //switches = (struct switch_ctx *)malloc(sizeof(struct switch_ctx));
    memset(switches, 0,sizeof(struct switch_ctx));
    printf("in value parse %p\n", switches);
    json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
        if (strcmp(key,"state_topic")==0) {
            if (json_object_get_type(val) == json_type_string) {
                printf("key: %s\n",key);
                printf("str: %s \nstrlen: %d\n",json_object_get_string(val),json_object_get_string_len(val));
                strncpy(switches->state_topic, json_object_get_string(val),json_object_get_string_len(val));
            }
        }
        if (strcmp(key,"command_topic")==0) {
            if (json_object_get_type(val) == json_type_string) {
                printf("key: %s\n",key);
                printf("str: %s \nstrlen: %d\n",json_object_get_string(val),json_object_get_string_len(val));
                strncpy(switches->cmd_topic, json_object_get_string(val),json_object_get_string_len(val));
            }
        }
        if (strcmp(key,"brightness_topic")==0) {
            if (json_object_get_type(val) == json_type_string) {
                printf("key: %s\n",key);
                printf("str: %s \nstrlen: %d\n",json_object_get_string(val),json_object_get_string_len(val));
                strncpy(switches->brightness_topic, json_object_get_string(val),json_object_get_string_len(val));
            }
        }
        if (strcmp(key,"brightness_state_topic")==0) {
            if (json_object_get_type(val) == json_type_string) {
                printf("key: %s\n",key);
                printf("str: %s \nstrlen: %d\n",json_object_get_string(val),json_object_get_string_len(val));
                strncpy(switches->brightness_state_topic, json_object_get_string(val),json_object_get_string_len(val));
            }
        }
        if (strcmp(key,"address")==0) {
            if (json_object_get_type(val) == json_type_int) {
                printf("key: %s\n",key);
                printf("int: %d\n",json_object_get_int(val));
                switches->address = json_object_get_int(val);
            }
        }
        if (strcmp(key,"device")==0) {
            if (json_object_get_type(val) == json_type_int) {
                printf("key: %s\n",key);
                printf("int: %d\n",json_object_get_int(val));
                switches->device = json_object_get_int(val);
            }
        }
    }
}

/*Parsing the json object*/
void json_parse(json_object * jobj,struct switch_ctx ** switches, int * num_switches) {
    enum json_type type;
    printf("Parsing config\n");


    json_object_object_foreach(jobj, key, val) { /*Passing through every array element*/
        if (strcmp("lights", key) == 0) {
            printf("%s found\n",key);
            type = json_object_get_type(val);
            if (type == json_type_array) {
                printf("parse lights array\n");
                json_parse_array(jobj, key, switches,num_switches);
            }
        }
    }
}
