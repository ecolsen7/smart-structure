#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <mosquitto.h>
#include <string.h>
#include "distance.h"
#include <time.h>

#define HOST_PORT 1883
#define KEEPALIVE 60
#define QOS 1
#define ECHO 2
#define TRIG 0
#define MAX_DIST 7000
#define NUM_US 5000
#define MIN_TIME 50
#define MAX_FNAME 30
#define MAX_SAMPLES 1000

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
   /*while (1) {
      lastDist = getDistance(ECHO, TRIG);  
   }*/
   return 0;
}

void mainLoop(struct mosquitto *mosq) {
   uint8_t d_flag = 0;
   int32_t dist, i, numDetect = 0;
   char *buf = calloc(1, MAX_FNAME);
   time_t tval = time(NULL), start_t, end_t;
   struct tm *curtime = localtime(&tval);
   double diff_t;
   time(&start_t);
   
   strcpy(buf, "test");
   strftime(buf + 4, MAX_FNAME, "%m-%d_%H-%M", curtime);
   strcpy(buf + strlen(buf), ".csv");
   //FILE *fd = fopen(buf, "w");

   while (1) {
      usleep(NUM_US);
      dist = getDistance(ECHO, TRIG);
      if (dist > 0 && dist < MAX_DIST) {
         d_flag = 1;
         numDetect++;
	 printf("%f  %d\n", dist / 58.8, numDetect);
      }  
      else if (dist > 0 && dist >= MAX_DIST) {
         if (d_flag) {
            if (numDetect >= MIN_TIME) {
               sendUpdate(mosq);
            }
            d_flag = 0;
            numDetect = 0;
         }
      }
   }
   /*for (i = 0; i < MAX_SAMPLES; i++) {
      dist = getDistance(ECHO, TRIG);
      //time(&end_t);
      //diff_t = difftime(end_t, start_t);
      //printf("%d\n", dist);
      fprintf(fd, "%d,%d\n", i, dist);
      usleep(NUM_US);
   }*/
   printf("Done\n");
   //fclose(fd);

}

void sendUpdate(struct mosquitto *mosq) { 
   char *topic, *msg;
   int msgid;

   topic = "test";
   msg = "2u";
   mosquitto_publish(mosq, &msgid, topic, strlen(msg), msg, QOS, true);  
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
