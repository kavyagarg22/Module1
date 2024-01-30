// Temperature Sensor and LED Control

// Define the analog pin to which the temperature sensor is connected
const int temperatureSensorPin = A0;
// Define the digital pin to which the LED is connected
const int ledPin = 13;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  // Set the temperature sensor pin as an input
  pinMode(temperatureSensorPin, INPUT);
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read the analog value from the temperature sensor
  int sensorValue = analogRead(temperatureSensorPin);
  // Convert the analog value to temperature in Celsius
  float temperature = (sensorValue * 5.0 / 1024) * 100;

  // Print the temperature to the Serial Monitor
  Serial.print("Current Temperature: ");
  Serial.println(temperature);

  // Check if the temperature is above a threshold (e.g., 25 degrees Celsius)
  if (temperature > 25) {
    // Turn on the LED if the temperature is above the threshold
    digitalWrite(ledPin, HIGH);
  } else {
    // Turn off the LED if the temperature is below or equal to the threshold
    digitalWrite(ledPin, LOW);
  }

  // Add a delay to control the update rate
  delay(500);
}
