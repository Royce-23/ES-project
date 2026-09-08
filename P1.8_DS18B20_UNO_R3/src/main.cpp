#include <Arduino.h>
#include <DallasTemperature.h>
#include <OneWire.h>

namespace {
constexpr uint8_t DS18B20_PIN = 2U;
constexpr uint8_t LED_PIN = 8U;
constexpr float LED_ON_TEMPERATURE_C = 30.0F;
constexpr float LED_OFF_TEMPERATURE_C = 28.0F;
constexpr unsigned long SAMPLE_PERIOD_MS = 1000UL;

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensor(&oneWire);
bool ledState = false;

void updateLedState(const float temperatureC)
{
    if (temperatureC >= LED_ON_TEMPERATURE_C) {
        ledState = true;
    } else if (temperatureC <= LED_OFF_TEMPERATURE_C) {
        ledState = false;
    }

    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
}

void printMeasurement(const float temperatureC)
{
    Serial.print(F("Temperature: "));
    Serial.print(temperatureC, 2);
    Serial.print(F(" C | LED: "));
    Serial.println(ledState ? F("ON") : F("OFF"));
}
}  // namespace

void setup()
{
    Serial.begin(9600);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    sensor.begin();
    sensor.setResolution(12);

    Serial.println(F("DS18B20 START"));
}

void loop()
{
    sensor.requestTemperatures();
    const float temperatureC = sensor.getTempCByIndex(0);

    if (temperatureC == DEVICE_DISCONNECTED_C) {
        Serial.println(F("Sensor error"));
        delay(SAMPLE_PERIOD_MS);
        return;
    }

    updateLedState(temperatureC);
    printMeasurement(temperatureC);
    delay(SAMPLE_PERIOD_MS);
}
