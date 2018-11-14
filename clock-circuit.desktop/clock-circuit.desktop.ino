/*
   Liam Telenko
   Nov/13/2018
   https://github.com/mailtelenko/Electromagnetic-Clock-Controller
   This software is to be used in conjunction with an electromagnetically powered clock.
   The program will repel the magnet on the pendulum of the clock whenever it passes over
   the magnetic coil connected to the Arduino.
*/

//
// Variables
//
// Pin references
int ledPin = 8; // Pin which controls the coil status LED
int adjustmentPot_length = A0; // Analog pin used to read the pulse length potentionmeter
int adjustmentPot_power = A1; // Analog pin used to read the coil power potentiometer
int adjustmentButton_faster = 7; // Pin used to detect faster button press
int adjustmentButton_slower = 6; // Pin used to detect slower button press

// Coil Pins
int coilPWMPin = 5; // PWM voltage setting pin for coil
int coilSwitch1 = 3; // Pin to set current direction on L293D
int coilSwitch2 = 4; // Pin to set current direction on L293D
int coilReadPin = A2; // Analog pin to read voltage from magnet passing over coil

//
// Control Variables
//
long pulseLength = 500; // The duration of the pulse to be applied to the pendulum --> Modified by calculateTiming
long pulseDelay = 67; // The length of the period of pulses
long coilThreshold = 1; // The analog value required to trigger checkMagnet
int coilPower = 0; // The value to be applied to the coil PWM pin

//
// General variables
//
// Timing variables
long currentTime = 0; // Global time
long periodStart = 0; // Start of the period
long nextPulse = 2000; // The time at which the next pulse shoudld start --> Modified by calculateTiming

/*
   setup method to initialize variables and pins. Runs once
   at startup.
*/
void setup() {
  Serial.begin(9600); //Open serial connection

  // Set pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(coilPWMPin, OUTPUT);
  pinMode(coilSwitch1, OUTPUT);
  pinMode(coilSwitch2, OUTPUT);
  pinMode(coilReadPin, INPUT);

  analogReference(INTERNAL); // Allow reading of coil
}
/*
   checkMagnet checks for a magnetic field from the copper coil
   and returns true if one is found.
*/
void checkMagnet() {
  // Shut off coil
  digitalWrite(coilSwitch1, LOW);
  digitalWrite(coilSwitch2, LOW);

  // Check coil values 3 times
  for (int x = 0; x < 10; x++) {
    // Check if there is no magnet over coil (false positive)
    if (analogRead(coilReadPin) <= coilThreshold)
      return; // Exit out of function
  }

  //Display period length on serial monitor for testing
  Serial.print("Period Length: ");
  Serial.println(millis() - periodStart);

  // Calculate pulse timing using period length
  calculateTiming(millis() - periodStart);

  // Reset periodStart
  periodStart = millis();

  pulseCoil(); // Call to pulseCoil to reduce latency
}

/*
   pulseCoil changes the behavior of the magnetic coil from input
   to output. The method is called when checkMagnet sets the next
   pulse time with calculateTiming.
   ------------------------------------------------------------------
   nextPulse --> long --> The time at which the coil should fire
   pulseDuration --> long --> The amount of time the coil should be on for
*/
void pulseCoil() {
  delay(pulseDelay);

  // Turn on coil switches
  digitalWrite(coilSwitch1, LOW);
  digitalWrite(coilSwitch2, HIGH);

  // Set coil power + LED
  digitalWrite(ledPin, HIGH); // Turn on LED
  analogWrite(coilPWMPin, coilPower); // Turn on power to coil

  delay(pulseLength);

  digitalWrite(ledPin, LOW);// Turn off LED
  analogWrite(coilPWMPin, 0); // Shut off power to coil
}

/*
   calculateTiming calculates the next time when a the coil should
   repel the magnet attached to the pendulum. The method adjusts
   for variations in the period of the swing to keep a consistant
   time.
   ---------------------------------------------------------------
*/
void calculateTiming(long periodLength) {
  /*
      If the pendulum is not correctly timed (millis() - periodStart)
      then set the next pulse accordingly (period too long --> later,
      short --> sooner)
  */
  if (periodLength < 500)
    Serial.println("Slower!");
  else if (periodLength > 500)
    Serial.println("Faster!");

  nextPulse = millis() + pulseDelay; // Set next pulse time
}

/*
   modifyTiming uses potentiometer input to change the duration of the
   pendulum swing.
*/
void modifyTiming() {
  // Read potentiometer values and apply to variables
  pulseLength = analogRead(adjustmentPot_length);
  coilPower = analogRead(adjustmentPot_power);

  // Check if the timing buttons have been pressed
  if (digitalRead(adjustmentButton_slower) == HIGH)
    pulseDelay += 1; // Increase length of delay
  else if (digitalRead(adjustmentButton_faster) == HIGH)
    pulseDelay -= 1; // Decrease length of delay

  // Print values to serial monitor
  Serial.print("Pulse Length: ");
  Serial.print(pulseLength);
  Serial.print(" | Power: ");
  Serial.print(coilPower);
  Serial.print(" | Pulse Delay: ");
  Serial.print(pulseDelay);
  Serial.print(" | Reading: ");
  Serial.println(analogRead(coilReadPin));
}

/*
   Main program loop. The code within this functio runs continuously.
*/
void loop() {
  currentTime = millis(); // Set current time to the amount of time since board turned on

  // Run function every 200ms
  if (currentTime % 200 == 0)
    modifyTiming(); // Check potentiometer to adjust timing of pendulum

  checkMagnet(); // Check if there is a magnet passing over the coil
  //pulseCoil(); // Check if coil should be pulsed
}
