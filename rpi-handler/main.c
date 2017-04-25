#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>
#include "distance.h"
#include <ctime>
#include <iostream>

#define HOST_PORT 1883
#define KEEPALIVE 60
#define QOS 1
#define ECHO 0
#define TRIG 2
#define MAX_DIST 20000
#define NUM_US 1000
#define MIN_TIME 1000
#define MAX_FNAME 30
void sendUpdate(struct mosquitto *mosq);
int mqttInit(struct mosquitto **mosq, char *host);
void mainLoop(struct mosquitto *mosq);

int main(int argc, char *argv[]) {
   struct mosquitto *mosq = NULL;
   char *host = "35.164.180.85";
   uint32_t lastDist = 0;

   setupHC(ECHO, TRIG);
   if (mqttInit(&mosq, host)) {
      return 1;
   }
   
   mainLoop(mosq);
   while (1) {
      lastDist = getDistance(ECHO, TRIG);  
   }
   return 0;
}

void mainLoop(struct mosquitto *mosq) {
   uint8_t numDetect = 0, d_flag = 0;
   uint32_t dist;
   char *buf = calloc(1, MAX_FNAME);
   time_t tval = time(NULL);
   struct tm *curtime = localtime(&tval);
   
   strcpy(buf, "test", 4);
   strftime(buf + 4, MAX_FNAME, "%m-%d_%H-%M", curtime);
   strcpy(buf + strlen(buf), ".csv", 4);
   int fd = fopen(buf, w);

   while (1) {
      dist = getDistance(ECHO, TRIG);
      if (dist && dist < MAX_DIST) {
         d_flag = 1;
         numDetect++;
         usleep(NUM_US);
      }
      else {
         if (d_flag) {
            if (numDetect >= MIN_TIME) {
               printf("Pass\n");
               sendUpdate(mosq);
            }
            d_flag = 0;
            numDetect = 0;
         }
      }
   }
}

void sendUpdate(struct mosquitto *mosq) { 
   char *topic, *msg;
   int msgid;

   topic = "test";
   msg = "\nObject passed";
   mosquitto_publish(mosq, &msgid, topic, strlen(msg), msg, QOS, true);  

   mosquitto_destroy(mosq);
   mosquitto_lib_cleanup();
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
