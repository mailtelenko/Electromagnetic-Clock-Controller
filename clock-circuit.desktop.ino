/*
   Liam Telenko
   Nov/08/2018
   https://github.com/mailtelenko/Electromagnetic-Clock-Controller
   This software is to be used in conjunction with an electromagnetically powered clock.
   The program will repel the magnet on the pendulum of the clock whenever it passes over
   the magnetic coil connected to the Arduino.
*/

//
// Variables
//
// Pin references
int ledPin = 8;
int adjustmentPot_timing = A0;
int adjustmentPot_power = A1;
int fasterButton = 7;
int slowerButton = 6;

// Coil Pins
int coilPWMPin = 5;
int coilSwitch1 = 3;
int coilSwitch2 = 4;
int coilReadPin = A2;

// Coil controls
int coilPower = 0;

// Timing variables
long currentTime = 0; // Global time
long periodStart = 0; // Start of the period
long nextPulse = 0; // The time at which the next pulse shoudld start --> Modified by calculateTiming
long pulseLength = 500; // The duration of the pulse to be applied to the pendulum --> Modified by calculateTiming
long pulseSpeed = 500; // The length of the period of pulses

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

  // Turn on coil
  digitalWrite(coilSwitch1, LOW);
  digitalWrite(coilSwitch2, HIGH);

  analogReference(INTERNAL);
}
/*
   checkMagnet checks for a magnetic field from the copper coil
   and returns true if one is found.
   ------------------------------------------------------------
   RETURN: boolean --> true if magnet passes over coil.
*/
bool checkMagnet() {
  //return analogRead(coilPin);
  return true;
}

/*
   calculateTiming calculates the next time when a the coil should
   repel the magnet attached to the pendulum. The method adjusts
   for variations in the period of the swing to keep a consistant
   time.
   ---------------------------------------------------------------
*/
void calculateTiming() {
  /*
      If the pendulum is not correctly timed (millis() - periodStart)
      then set the next pulse accordingly (period too long --> later, short --> sooner)
  */
  if (periodStart + pulseLength < currentTime)
    nextPulse = currentTime + (pulseSpeed - pulseLength);
}

/*
   pulseCoil changes the behavior of the magnetic coil from input
   to output. The method waits until a predfined time defined within
   calculateTiming.
   ------------------------------------------------------------------
   nextPulse --> long --> The time at which the coil should fire
   pulseDuration --> long --> The amount of time the coil should be on for
*/
void pulseCoil() {
  // Check if the current time is when the pulse should be fired
  // and if the time is within the length it sould be fired for.
  if (millis() >= nextPulse && millis() <= nextPulse + pulseLength) {
    digitalWrite(ledPin, HIGH); // Turn on LED
    analogWrite(coilPWMPin, coilPower); // Turn on power to coil
  } else {
    digitalWrite(ledPin, LOW);// Turn off LED
    analogWrite(coilPWMPin, 0); // Shut off power to coil
  }

}

/*
   modifyTiming uses potentiometer input to change the duration of the
   pendulum swing.
*/
void modifyTiming() {
  pulseLength = analogRead(adjustmentPot_timing);
  coilPower = analogRead(adjustmentPot_power);

  if(digitalRead(fasterButton) == HIGH)
    pulseSpeed += 1;
  else if(digitalRead(slowerButton) == HIGH)
    pulseSpeed -= 1;

  Serial.print("Pulse Length: ");
  Serial.print(pulseLength);
  Serial.print(" | Power: ");
  Serial.print(coilPower);
  Serial.print(" | Period: ");
  Serial.print(pulseSpeed);
  Serial.print(" | Reading: ");
  Serial.println(analogRead(coilReadPin));
}

/*
   Main program loop. The code within this functio runs continuously.
*/
void loop() {
  currentTime = millis(); // Set current time to the amount of time since board turned on

  // if checkMagnet() is true --> calculate when to pulse magnet + strength based on time of swing
  if (checkMagnet() && nextPulse + pulseLength < millis())
    calculateTiming(); // Calculate next time to pulse coil

  if (currentTime % 200 == 0)
    modifyTiming(); // Check potentiometer to adjust timing of pendulum
  pulseCoil(); // Run pulseCoil to check if coil should be on
}
