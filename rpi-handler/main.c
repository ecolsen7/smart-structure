#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#define HOST_PORT 1883
#define KEEPALIVE 60
#define QOS 1

int mqtt_init(struct mosquitto **mosq, char *host) {
   mosquitto_lib_init();
   *mosq = mosquitto_new(NULL, true, NULL);
   if (!(*mosq)) {
      return -1;
   }
   
   if (mosquitto_connect(*mosq, host, HOST_PORT, KEEPALIVE)) {
      return -2;
   }
   return 0;
}

int main(int argc, char *argv[]) {
   struct mosquitto *mosq = NULL;
   char *host = "35.164.180.85", *topic, *msg;
   int msgid;

   if (mqtt_init(&mosq, host)) {
      return 1;
   }
    
   topic = "test";
   msg = "\ntest MQTT message from C client";
   mosquitto_publish(mosq, &msgid, topic, strlen(msg), msg, QOS, true);  

   mosquitto_destroy(mosq);
   mosquitto_lib_cleanup();
   return 0;
}
