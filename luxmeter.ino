#include <Wire.h>
#include "Adafruit_TSL2591.h"

// Constants
const unsigned long TEN_SECONDS = 10000;            // 10 seconds in milliseconds
const unsigned long ONE_HOUR = 60 * 60 * 1000;      // 1 hour in milliseconds
const unsigned long TWO_HOURS = 2 * ONE_HOUR;       // 2 hours in milliseconds

// Create an instance of the Adafruit_TSL2591 class
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

// Variables to track lux warning state and timing
bool luxWarningShown = false;
unsigned long luxWarningStartTime = 0;

void setup() {
  Serial.begin(9600);

  // Initialize the TSL2591 sensor
  if (!tsl.begin()) {
    Serial.println("Couldn't find TSL2591 sensor!");
    while (1);
  }

  // Configure the gain and integration time
  tsl.setGain(TSL2591_GAIN_MED);
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);
}

void loop() {
  // Read the luminosity (lux) from the sensor
  uint32_t lum = tsl.getFullLuminosity();

  // Extract lux values from luminosity data
  uint16_t broadband = lum >> 16;
  uint16_t infrared = lum & 0xFFFF;

  // Calculate lux using sensor-specific algorithm
  float lux = tsl.calculateLux(broadband, infrared);

  // Print lux value to Serial Monitor
  Serial.print("Lux: ");
  Serial.println(lux);

  // Check if lux value exceeds 100
  if (lux > 100) {
    Serial.println("Warning: Lux value exceeds 100!");
  }

  // Check if lux value has been continuously high for specific durations
  if (lux > 100) {
    // If lux exceeds 100, check if warning has already been shown
    if (!luxWarningShown) {
      // If warning has not been shown, update the warning state and start time
      luxWarningShown = true;
      luxWarningStartTime = millis();
    } else {
      // If warning has been shown, check if it has been continuously high for specific durations
      unsigned long elapsedTime = millis() - luxWarningStartTime;
      if (elapsedTime >= TEN_SECONDS && elapsedTime < ONE_HOUR) {
        Serial.println("Warning: Lux value has been high for 10 seconds!");
        // Additional actions can be added here, such as sending a notification
      } else if (elapsedTime >= ONE_HOUR && elapsedTime < TWO_HOURS) {
        Serial.println("Warning: Lux value has been high for 1 hour!");
        // Additional actions can be added here, such as sending a notification
      } else if (elapsedTime >= TWO_HOURS) {
        Serial.println("Warning: Lux value has been high for 2 hours!");
        // Additional actions can be added here, such as sending a notification
      }
    }
  } else {
    // Reset warning state and start time if lux value is not high
    luxWarningShown = false;
    luxWarningStartTime = 0;
  }

  delay(1000); // Wait for 1 second before taking next reading
}