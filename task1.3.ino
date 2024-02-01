// Temperature Sensor, IR Sensor, and LED Control with Interrupts

// Pin configurations
const int temperatureSensorPin = A0;
const int irSensorPin = A1;     // Pin to which the IR sensor is connected
const int ledPin = 13;
const int irLedPin = 12;        // LED controlled by the IR sensor

// Time interval for temperature monitoring in milliseconds
const unsigned long interval = 1000;

// Variable to store the last time the temperature was checked
unsigned long previousMillis = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);

  // Set pin modes
  pinMode(temperatureSensorPin, INPUT);
  pinMode(irSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(irLedPin, OUTPUT);

  // Set up a timer interrupt for temperature and IR sensor checks
  noInterrupts();                  // Disable interrupts during configuration
  TCCR1A = 0;
  TCCR1B = 0;
  OCR1A = 15624;                    // Set the compare register value for 1 second interval
  TCCR1B |= (1 << WGM12);           // Configure timer in CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Set prescaler to 1024
  TIMSK1 |= (1 << OCIE1A);          // Enable timer compare interrupt
  interrupts();                     // Enable interrupts after configuration
}

void loop() {
  // Nothing to do in the loop because the temperature and IR checks are handled by interrupts
}

// Timer1 compare interrupt service routine
ISR(TIMER1_COMPA_vect) {
  // Read the analog value from the temperature sensor
  int temperatureSensorValue = analogRead(temperatureSensorPin);
  float temperature = (temperatureSensorValue * 5.0 / 1024) * 100;

  // Print the temperature to the Serial Monitor
  Serial.print("Temperature Sensor - Current Temperature: ");
  Serial.println(temperature);

  // Check if the temperature is above a threshold (e.g., 25 degrees Celsius)
  if (temperature > 25) {
    digitalWrite(ledPin, HIGH);    // Turn on the LED if the temperature is above the threshold
  } else {
    digitalWrite(ledPin, LOW);     // Turn off the LED if the temperature is below or equal to the threshold
  }

  // Read the digital value from the IR sensor
  int irSensorValue = digitalRead(irSensorPin);

  // Print the IR sensor value to the Serial Monitor
  Serial.print("IR Sensor - Current Value: ");
  Serial.println(irSensorValue);

  // Check if the IR sensor value is HIGH (object detected)
  if (irSensorValue == HIGH) {
    digitalWrite(irLedPin, HIGH);  // Turn on the IR LED if an object is detected
  } else {
    digitalWrite(irLedPin, LOW);   // Turn off the IR LED if no object is detected
  }
}


