#include <stdio.h>
#include <stdlib.h>
#include <mosquitto.h>
#include <string.h>

#define HOST_PORT 1883
#define KEEPALIVE 60
#define QOS 1
#define ECHO 0
#define TRIG 2

void sendUpdate(struct mosquitto *mosq);
int mqttInit(struct mosquitto **mosq, char *host);

int main(int argc, char *argv[]) {
   struct mosquitto *mosq = NULL;
   uint32_t lastDist = 0;

   setupHC(ECHO, TRIG);
   if (mqtt_init(&mosq, host)) {
      return 1;
   }
   
   mainLoop(mosq);
   while (1) {
      lastDist = getDistance(ECHO);  
   }
}

void mainLoop(struct mosquitto *mosq) {
   char *host = "35.164.180.85";

}

void sendUpdate(struct mosquitto *mosq, char *host) { 
   char *topic, *msg;
   int msgid;

   topic = "test";
   msg = "\ntest MQTT message from C client";
   mosquitto_publish(mosq, &msgid, topic, strlen(msg), msg, QOS, true);  

   mosquitto_destroy(mosq);
   mosquitto_lib_cleanup();
   return 0;
}

int mqttInit(struct mosquitto **mosq, char *host) {
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
