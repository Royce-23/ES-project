#pragma once
#include <stdint.h>
constexpr uint8_t HIGH = 1;
constexpr uint8_t LOW = 0;
constexpr uint8_t OUTPUT = 1;
constexpr uint8_t INPUT_PULLUP = 2;
extern uint8_t mockPins[32];
extern uint8_t mockModes[32];
extern int mockAnalog[32];
extern uint32_t mockTime;
inline void pinMode(uint8_t pin, uint8_t mode) { mockModes[pin] = mode; }
inline uint8_t digitalRead(uint8_t pin) { return mockPins[pin]; }
inline void digitalWrite(uint8_t pin, uint8_t value) { mockPins[pin] = value; }
inline int analogRead(uint8_t pin) { return mockAnalog[pin]; }
inline unsigned long millis() { return mockTime; }
constexpr uint8_t A0 = 14;
constexpr uint8_t A1 = 15;
