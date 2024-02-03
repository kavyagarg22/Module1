const uint8_t BUTTON_PIN = 2;  // Pin for the button input
const uint8_t LED_PIN = 13;    // Pin for the LED output

uint8_t buttonPrevState = LOW; // Previous button state
uint8_t ledState = LOW;        // Current LED state (LOW = off, HIGH = on)

// Variables to help with debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;  

void setup()
{
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Set button pin as input with pull-up resistor
  pinMode(LED_PIN, OUTPUT);          // Set LED pin as output
  Serial.begin(9600);                // Initialize serial communication
}

void loop()
{
  uint8_t buttonState = digitalRead(BUTTON_PIN); // Read button state

  // Print button state, previous button state, and LED state
  Serial.print("Button: ");
  Serial.print(buttonState);
  Serial.print(" | Prev: ");
  Serial.print(buttonPrevState);
  Serial.print(" | LED: ");
  Serial.println(ledState);

  // Check if the button state has changed and debounce it
  if (buttonState != buttonPrevState && (millis() - lastDebounceTime) > debounceDelay)
  {
    ledState = !ledState; // Toggle LED state
    digitalWrite(LED_PIN, ledState); // Update LED output
    lastDebounceTime = millis(); // Save the current time for the next debounce
  }

  buttonPrevState = buttonState; // Update previous button state

  delay(50); 
}
