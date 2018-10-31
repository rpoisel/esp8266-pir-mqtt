#include "Arduino.h"
#include "mock.h"

#include <iostream>

void pinMode(uint8_t pin, uint8_t mode) {}

void digitalWrite(uint8_t pin, uint8_t val) {}
int digitalRead(uint8_t pin) { return LOW; }

unsigned long millis(void)
{
  if (millisImpl)
  {
    return millisImpl();
  }

  std::cerr << "Implementation for millis() has not been set." << std::endl;

  return 0;
}

unsigned long micros(void) { return 0; }

void delay(unsigned long timespan)
{
  // do nothing when mocked
}
