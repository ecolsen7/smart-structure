#include <wiringPi.h>
#include <time.h>

#define TTL_DELAY 10

void setupHC(uint8_t echo, uint8_t trig) {
   wiringPiSetup();
   pinMode(echo, INPUT);
   pinMode(trig, OUTPUT);
}

void sendTTL(uint8_t pin) {
   digitalWrite(pin, HIGH);
   delayMicroseconds(TTL_DELAY);
   digitalWrite(pin, LOW);
}

uint32_t getDistance(uint8_t pin) {
   timeval start, end, diff;
   uint16_t tv_ret;

   sendTTL(pin);
   while (!digitalRead(pin)) {
      tv_ret = gettimeofday(&start, NULL);
   }
   while (digitalRead(pin) == 1) {
      tv_ret = gettimeofday(&end, NULL);
   }
   timersub(&end, &start, &diff);
   return (uint32_t)(diff.tv_usec);
}

void waitForBtn(uint8_t btn) {
   while(!digitalRead(btn))
   return;
}
