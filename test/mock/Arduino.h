#ifndef ARDUINO_H
#define ARDUINO_H

#include <cstdint>

#define INTPUT 0x00
#define OUTPUT 0x01
#define HIGH 0x1
#define LOW 0x0

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);

unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ARDUINO_H */
