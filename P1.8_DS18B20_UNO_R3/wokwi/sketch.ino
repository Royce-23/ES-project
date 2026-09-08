#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_PIN 2
#define LED_PIN 8

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensor(&oneWire);

bool ledState = false;

void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  sensor.begin();
  sensor.setResolution(12);

  Serial.println("DS18B20 START");
}

void loop() {
  sensor.requestTemperatures();

  float temperature = sensor.getTempCByIndex(0);

  if (temperature == DEVICE_DISCONNECTED_C) {
    Serial.println("Sensor error");
    delay(1000);
    return;
  }

  if (temperature >= 30.0) {
    ledState = true;
  }

  if (temperature <= 28.0) {
    ledState = false;
  }

  digitalWrite(LED_PIN, ledState ? HIGH : LOW);

  Serial.print("Temperature: ");
  Serial.print(temperature, 2);
  Serial.print(" C | LED: ");
  Serial.println(ledState ? "ON" : "OFF");

  delay(1000);
}

