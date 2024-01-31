// Temperature Sensor and LED Control with Interrupts

// Define the analog pin to which the temperature sensor is connected
const int temperatureSensorPin = A0;
// Define the digital pin to which the LED is connected
const int ledPin = 13;

// Define the time interval for temperature monitoring in milliseconds
const unsigned long interval = 1000; // 1 second

// Variable to store the last time the temperature was checked
unsigned long previousMillis = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  // Set the temperature sensor pin as an input
  pinMode(temperatureSensorPin, INPUT);
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);

  // Set up a timer interrupt to check temperature periodically
  noInterrupts(); // Disable interrupts during configuration
  // Set up Timer1 to generate an interrupt every interval milliseconds
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 15624;  // Set the compare register value for 1 second interval
  TCCR1B |= (1 << WGM12);  // Configure timer in CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Set prescaler to 1024
  TIMSK1 |= (1 << OCIE1A);  // Enable timer compare interrupt
  interrupts();  // Enable interrupts after configuration
}

void loop() {
  // Nothing to do in the loop because the temperature check is handled by interrupts
}

// Timer1 compare interrupt service routine
ISR(TIMER1_COMPA_vect) {
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
} 


