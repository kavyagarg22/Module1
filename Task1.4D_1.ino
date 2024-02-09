const int motionSensorPin = 2;        // Pin for the first motion sensor
const int buzzerPin = 7;
const int tiltSensorPin = 3;
const int motionSensor2Pin = 4;       // Pin for the second motion sensor

int redLedPin = 13;                   // Red LED
int blueLedPin = 11;                  // Blue LED
const int greenLedPin = 12;           // Green LED on pin D12

volatile uint8_t motionState, tiltState, motionState2;

volatile uint8_t prevMotionState, prevMotionState2, prevTiltState;

const uint16_t timerInitialValue = 0;  // Counter initial value
const uint16_t timerCompareValue = 31250; // Compare thresholds

void setup() {
  Serial.begin(9600);               // Initialize serial communication

  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
  pinMode(tiltSensorPin, INPUT_PULLUP);
  pinMode(motionSensorPin, INPUT);      // Initialize the first motion sensor as an input
  pinMode(motionSensor2Pin, INPUT);     // Initialize the second motion sensor as an input
  pinMode(greenLedPin, OUTPUT);

  // Enable Pin Change Interrupts (PCINT) for specific pins
  PCICR |= 0b00000111;               // Enable PCIE0, PCIE1, and PCIE2
  PCMSK2 |= 0b10011100;              // Enable PCINT19 (D4), PCINT20 (D5), and PCINT21 (D6)

  // Configure Timer1 for interrupt
  TCCR1A = 0;
  TCCR1B |= (1 << WGM12) | (1 << CS12); // Set CTC mode and prescaler to 256
  TCCR1B &= ~((1 << CS11) | (1 << CS10));

  TCNT1 = timerInitialValue;         // Set initial value of the counter
  OCR1A = timerCompareValue;         // Set the compare threshold

  TIMSK1 = (1 << OCIE1A);            // Enable Timer1 compare match interrupt

  // Enable global interrupts
  sei();
}

// Infinite loop
void loop() {
  if (prevMotionState != motionState) {
    digitalWrite(redLedPin, motionState);
    Serial.println("Motion Sensor 1 Interrupt - Turned Red LED");
    prevMotionState = motionState;
  }

  if (prevMotionState2 != motionState2) {
    digitalWrite(blueLedPin, motionState2);
    Serial.println("Motion Sensor 2 Interrupt - Turned Blue LED");
    prevMotionState2 = motionState2;
  }

  if (prevTiltState != tiltState) {
    digitalWrite(buzzerPin, tiltState);
    Serial.println("Tilt Sensor Interrupt - Buzzed");
    prevTiltState = tiltState;
  }

  delay(200);
}

// Pin Change Interrupt Service Routine for PCINT19, PCINT20, and PCINT21
ISR(PCINT2_vect) {
  motionState = PIND & (1 << PIND4);  // Read the state of the first motion sensor (D4)
  motionState2 = PIND & (1 << PIND2); // Read the state of the second motion sensor (D2)
  tiltState = PIND & (1 << PIND3);    // Read the state of the tilt sensor (D3)
}

// Timer1 Compare Match Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
  PORTB ^= (1 << PORTB4);          // Toggle the state of the green LED
}
