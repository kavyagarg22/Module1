// Pin configuration for the button
const uint8_t BTN_PIN = 2;

// Pin configuration for the LED
const uint8_t LED_PIN = 13;

// Variable to store the previous state of the button
uint8_t buttonPrevState = LOW;

// Variable to store the state of the LED
uint8_t ledState = LOW;

// Setup function: runs once at the beginning
void setup()
{
  // Configure the button pin as input with an internal pull-up resistor
  pinMode(BTN_PIN, INPUT_PULLUP);

  // Configure the LED pin as output
  pinMode(LED_PIN, OUTPUT);

  // Initialize serial communication for debugging
  Serial.begin(9600);
}

// Loop function: runs repeatedly
void loop()
{
  // Read the current state of the button
  uint8_t buttonState = digitalRead(BTN_PIN);
  
  // Debugging: Print button state, previous button state, and LED state
  Serial.print(buttonState);
  Serial.print(buttonPrevState);
  Serial.print(ledState);
  Serial.println("");
  
  // Check if the button state has changed
  if(buttonState != buttonPrevState)
  {
    // Toggle the LED state
    ledState = !ledState;

    // Update the LED state
    digitalWrite(LED_PIN, ledState);
  }
  
  // Update the previous button state for the next iteration
  buttonPrevState = buttonState;
    
  // Introduce a delay to debounce the button and avoid rapid state changes
  delay(500);
}

