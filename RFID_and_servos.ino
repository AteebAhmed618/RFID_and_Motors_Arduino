#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

// RFID module connections (SPI protocol)
const int RST_PIN = 9;   // RFID RST pin
const int SS_PIN = 10;   // RFID SDA/SS pin (Slave Select)

// Joystick and LED pins
const int joystickPinX = A0;  // Joystick's X-axis for Motor 1.
const int joystickPinY = A1;  // Joystick's Y-axis for Motor 2.
const int ledStatusPin = 2;   // A single LED to indicate status.

// Motor pins for the two drive motors. They must be PWM pins.
const int motorOnePin = 5;
const int motorTwoPin = 6;

// Create Servo objects for the two motors.
Servo motorOne;
Servo motorTwo;

// Create an MFRC522 object to interact with the RFID module.
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Variables to store the joystick values.
int joystickValueX = 0;
int joystickValueY = 0;

// Set a threshold for the center and outer positions of the joystick.
const int NEUTRAL_ZONE_MIN = 400;
const int NEUTRAL_ZONE_MAX = 600;

// A boolean flag to act as a toggle switch for motor control.
bool isEnabled = false;
bool isBlinking = false;
unsigned long previousMillis = 0;
const long interval = 500;
bool ledState = LOW;

// Define a master card UID for authentication.
byte masterCardUID[] = {0x83, 0x1D, 0x63, 0x10};

void setup() {
  // Attach the Servo objects to their respective pins.
  // We use standard servo pins for continuous rotation servos.
  motorOne.attach(motorOnePin);
  motorTwo.attach(motorTwoPin);

  // Configure the LED pin as an OUTPUT.
  pinMode(ledStatusPin, OUTPUT);

  // Initialize the Serial Monitor for debugging.
  Serial.begin(9600);

  // Initialize the SPI communication and the RFID module.
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("RFID Reader initialized!");
}

void loop() {
  // Check for a new RFID card.
  if (mfrc522.PICC_IsNewCardPresent()) {
    if (mfrc522.PICC_ReadCardSerial()) {
      Serial.println("Card detected!");
      
      bool isMasterCard = true;
      if (mfrc522.uid.size != 4) {
        isMasterCard = false;
      } else {
        for (byte i = 0; i < mfrc522.uid.size; i++) {
          if (mfrc522.uid.uidByte[i] != masterCardUID[i]) {
            isMasterCard = false;
            break;
          }
        }
      }

      if (isMasterCard) {
        isEnabled = !isEnabled;
        isBlinking = false;
        Serial.print("Access ");
        Serial.println(isEnabled ? "Granted" : "Revoked");
        
        digitalWrite(ledStatusPin, HIGH);
        delay(500);
        digitalWrite(ledStatusPin, LOW);

      } else {
        isBlinking = true;
        Serial.println("Access Denied! Security Breach!");
      }
      mfrc522.PICC_HaltA();
    }
  }

  // --- Motor and LED Control Logic ---
  if (isEnabled) {
    joystickValueX = analogRead(joystickPinX);
    joystickValueY = analogRead(joystickPinY);

    // Control Motor 1 with the X-axis of the joystick.
    if (joystickValueX > NEUTRAL_ZONE_MAX) {
      // Positive X (Right): Spin motor 1 clockwise.
      int speed = map(joystickValueX, NEUTRAL_ZONE_MAX, 1023, 90, 180);
      motorOne.write(speed);
    } else if (joystickValueX < NEUTRAL_ZONE_MIN) {
      // Negative X (Left): Spin motor 1 anti-clockwise.
      int speed = map(joystickValueX, NEUTRAL_ZONE_MIN, 0, 90, 0);
      motorOne.write(speed);
    } else {
      // Center X: Stop motor 1.
      motorOne.write(90);
    }

    // Control Motor 2 with the Y-axis of the joystick.
    if (joystickValueY > NEUTRAL_ZONE_MAX) {
      // Positive Y (Up): Spin motor 2 clockwise.
      int speed = map(joystickValueY, NEUTRAL_ZONE_MAX, 1023, 90, 180);
      motorTwo.write(speed);
    } else if (joystickValueY < NEUTRAL_ZONE_MIN) {
      // Negative Y (Down): Spin motor 2 anti-clockwise.
      int speed = map(joystickValueY, NEUTRAL_ZONE_MIN, 0, 90, 0);
      motorTwo.write(speed);
    } else {
      // Center Y: Stop motor 2.
      motorTwo.write(90);
    }
    
    // The LED lights up and stays on when the system is enabled.
    digitalWrite(ledStatusPin, HIGH);

  } else {
    // Stop all motors if the system is not enabled.
    motorOne.write(90);
    motorTwo.write(90);
    
    // Handle the blinking logic if the isBlinking flag is true.
    if (isBlinking) {
      unsigned long currentMillis = millis();
      if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledStatusPin, ledState);
      }
    } else {
      // Keep the LED off when the system is not enabled and not blinking.
      digitalWrite(ledStatusPin, LOW);
    }
  }
}
