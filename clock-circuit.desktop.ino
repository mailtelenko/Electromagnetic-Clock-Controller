/*
   Liam Telenko
   Nov/06/2018
   This software is to be used in conjunction with an electromagnetically powered clock.
   The program will repel the magnet on the pendulum of the clock whenever it passes over
   the magnetic coil connected to the Arduino.
*/

//
// Variables
//
// Pin references
int ledPin = 2;
int coilPin = A5;

// Timing variables
long currentTime = 0; // Global time 
long periodStart = 0; // Start of the period 
long nextPulse = 0; // The time at which the next pulse shoudld start

void setup() {
  Serial.begin(9600); //Open serial connection

  // Set pin modes
  pinMode(ledPin, OUTPUT);
  pinMode(coilPin, INPUT);
}

/*
   checkMagnet checks for a magnetic field from the copper coil
   and returns true if one is found.
   ------------------------------------------------------------
   RETURN: boolean --> true if magnet passes over coil.
*/
bool checkMagnet() {
  //return analogRead(coilPin);
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
}

/*
   pulseCoil changes the behavior of the magnetic coil from input
   to output. The method waits until a predfined time defined within
   calculateTiming.
   ------------------------------------------------------------------
   nextPulse --> long --> The time at which the coil should fire
*/
void pulseCoil() {

}

/*
   Main program loop. The code within this functio runs continuously.
*/
void loop() {
  currentTime = millis(); // Set current time to the amount of time since board turned on

  // if checkMagnet() is true --> calculate when to pulse magnet + strength based on time of swing
  if (checkMagnet() && nextPulse < millis())
    calculateTiming();

  pulseCoil();

  //Serial.println(checkMagnet()); // Check if a magnet can be detected
  Serial.println(analogRead(coilPin));

}
