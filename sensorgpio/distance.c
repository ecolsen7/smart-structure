#include <wiringPi.h>
#include <time.h>
#define TTL_DELAY 10

void sendTTL(uint8_t pin) {
   digitalWrite(pin, HIGH);
   delayMicroseconds(TTL_DELAY);
   digitalWrite(pin, LOW);
}

uint32_t getDistance(uint8_pin) {
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
