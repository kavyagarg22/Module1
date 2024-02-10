// C++ code
/*
  This program blinks the LED on pin 13 when there is motion detected by the IR sensor.
  Three LEDs are added to show the output of the IR sensor, temperature sensor, and motion sensor.
  The IR sensor LED blinks when the analog value from the temperature sensor is HIGH.
  The Temperature sensor LED blinks when the temperature sensor value is HIGH.
  A potentiometer is used to adjust the sensitivity of the IR sensor.
*/

// Pins
const int led_pin = 13;          // Use built-in LED on pin 13
const int ir_sensor_pin = A1;    // Connect IR sensor to analog pin A1
const int motion_pin = 12;
const int temperature_pin = A0;

// Additional LEDs
const int ir_led_pin = 7;        // Connect an LED to pin 7 for IR sensor output
const int temperature_led_pin = 8; // Connect an LED to pin 8 for temperature sensor output
const int motion_led_pin = 9;     // Connect an LED to pin 9 for motion sensor output

// Potentiometer
const int pot_pin = A2;          // Connect potentiometer to analog pin A2

// counter and compare values
const uint16_t t1_load = 0;
const uint16_t t1_comp = 31250;

void setup()
{
  Serial.begin(9600);

  // Set LED pins as output
  pinMode(led_pin, OUTPUT);
  pinMode(ir_led_pin, OUTPUT);
  pinMode(temperature_led_pin, OUTPUT);
  pinMode(motion_led_pin, OUTPUT);

  // reset timer1 control reg A
  TCCR1A = 0;

  // set CTC mode
  TCCR1B &= ~(1 << WGM13);
  TCCR1B |= (1 << WGM12);

  // set to prescaler of 256
  TCCR1B |= (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  // reset timer1 and compare value
  TCNT1 = t1_load;
  OCR1A = t1_comp;

  // enable timer1 compare interrupt
  TIMSK1 = (1 << OCIE1A);

  pinMode(motion_pin, INPUT);
  pinMode(temperature_pin, INPUT);
  pinMode(ir_sensor_pin, INPUT);
  
  // Enable global interrupts
  sei();
}

int motionResult = 0;
int irSensorValue = 0;
int temperatureSensorValue = 0;
int sensitivityValue = 0;

void loop()
{
  motionResult = digitalRead(motion_pin);
  irSensorValue = analogRead(ir_sensor_pin);
  temperatureSensorValue = analogRead(temperature_pin);
  sensitivityValue = analogRead(pot_pin);

  // Adjust the sensitivity of the IR sensor based on the potentiometer value
  irSensorValue = map(irSensorValue, 0, 1023, 0, sensitivityValue);

  // Blink the IR LED if the IR sensor value is HIGH
  if (irSensorValue > 200) {
    digitalWrite(ir_led_pin, HIGH);
    delay(500);  // Adjust the delay time as needed
    digitalWrite(ir_led_pin, LOW);
    delay(500);
  } else {
    digitalWrite(ir_led_pin, LOW);
  }

  // Blink the temperature LED if the temperature sensor value is high
  if (temperatureSensorValue > 200) {
    digitalWrite(temperature_led_pin, HIGH);
    delay(500);  // Adjust the delay time as needed
    digitalWrite(temperature_led_pin, LOW);
    delay(500);
  } else {
    digitalWrite(temperature_led_pin, LOW);
  }

  digitalWrite(motion_led_pin, motionResult);

  // Print the output of all sensors to the Serial Monitor
  Serial.print("Motion Sensor: ");
  Serial.print(motionResult);
  Serial.print(" | IR Sensor: ");
  Serial.print(irSensorValue);
  Serial.print(" | Temperature Sensor: ");
  Serial.println(temperatureSensorValue);

  delay(1000); // Wait for 1000 milliseconds
}

ISR(TIMER1_COMPA_vect)
{
  // Blink the main LED on pin 13
  PORTB ^= (1 << led_pin);
}


