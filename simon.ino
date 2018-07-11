#define MAX_ROUND 100

#define RED 1
#define GREEN 2
#define YELLOW 4
#define BLUE 8

const int redButtonPin = 2;
const int greenButtonPin = 3;
const int yellowButtonPin = 4;
const int blueButtonPin = 5;

const int redLedPin = 6;
const int greenLedPin = 7;
const int yellowLedPin = 8;
const int blueLedPin = 9;

byte gameData[MAX_ROUND];
int roundNumber = -1;

void setup() {
  // Pin setup
  pinMode(redButtonPin, INPUT_PULLUP);
  pinMode(greenButtonPin, INPUT_PULLUP);
  pinMode(yellowButtonPin, INPUT_PULLUP);
  pinMode(blueButtonPin, INPUT_PULLUP);

  pinMode(redLedPin, OUTPUT);
  digitalWrite(redLedPin, LOW);
  pinMode(greenLedPin, OUTPUT);
  digitalWrite(greenLedPin, LOW);
  pinMode(yellowLedPin, OUTPUT);
  digitalWrite(yellowLedPin, LOW);
  pinMode(blueLedPin, OUTPUT);
  digitalWrite(blueLedPin, LOW);
  
  // Init game data. Put 0 in all values
  for (int i=0; i<MAX_ROUND; i++) {
    gameData[i] = 0;
  }

  delay(1000);

  // Pseudo random
  randomSeed(analogRead(A7));
}

void newRound()
{
  roundNumber++;
  byte choices[4] = {RED, GREEN, YELLOW, BLUE};
  gameData[roundNumber] = choices[random(4)];
}

void lightAndSound(byte key, bool on, bool sound = true)
{
  if (!on && sound) {
    noTone(10);
  }
  if (key & RED) {
    digitalWrite(redLedPin, on ? HIGH : LOW);
    if (on && sound) {
      tone(10, 262);
    }
  }
  if (key & GREEN) {
    digitalWrite(greenLedPin, on ? HIGH : LOW);
    if (on && sound) {
      tone(10, 330);
    }
  }
  if (key & YELLOW) {
    digitalWrite(yellowLedPin, on ? HIGH : LOW);
    if (on && sound) {
      tone(10, 392);
    }
  }
  if (key & BLUE) {
    digitalWrite(blueLedPin, on ? HIGH : LOW);
    if (on && sound) {
      tone(10, 523);
    }
  }
}

void loose(byte expectedButton)
{
  lightAndSound(expectedButton, true, false);
  tone(10, 698);
  delay(500);
  
  lightAndSound(expectedButton, false, false);
  tone(10, 659);
  delay(500);

  lightAndSound(expectedButton, true, false);
  tone(10, 622);
  delay(500);


  lightAndSound(expectedButton, false, false);
  tone(10, 587);
  delay(500);
  lightAndSound(expectedButton, true, false);
  delay(500);
  lightAndSound(expectedButton, false, false);
  delay(500);
  noTone(10);

  delay(500);

  roundNumber = -1;
}

void loop() {
  newRound();

  // Replay all rounds
  for (int i=0; i<=roundNumber; i++) {
    lightAndSound(gameData[i], true);
    delay(500);
    lightAndSound(gameData[i], false);
    delay(500);
  }

  // Wait for user input
  for (int i=0; i<=roundNumber; i++) {
    bool redPressed = false, greenPressed = false, yellowPressed = false, bluePressed = false;
    unsigned int forgetTimer = 0;
    do
    {
      delay(1);
      if (++forgetTimer >= 60000) {
        whistleSamsung();
        forgetTimer = 0;
      }

      redPressed = digitalRead(redButtonPin) == LOW;
      greenPressed = digitalRead(greenButtonPin) == LOW;
      yellowPressed = digitalRead(yellowButtonPin) == LOW;
      bluePressed = digitalRead(blueButtonPin) == LOW;
    } while (!redPressed && !greenPressed && !yellowPressed && !bluePressed);

    byte pressed = (((byte)bluePressed) << 3) + (((byte)yellowPressed) << 2) + (((byte)greenPressed) << 1) + ((byte)redPressed);

    if (pressed != gameData[i]) {
      loose(gameData[i]);
      return;
    }

    lightAndSound(pressed, true);
    delay(250);

    // 250ms min, keep light + sound ON until btn is released
    while (digitalRead(redButtonPin) == LOW || digitalRead(greenButtonPin) == LOW || digitalRead(yellowButtonPin) == LOW || digitalRead(blueButtonPin) == LOW) {
      delay(1);
    }
    
    lightAndSound(pressed, false);
  }

  delay(1000);
}

void whistleSamsung()
{
  tone(10, 988);
  delay(200);
  
  tone(10, 1480);
  delay(200);
  
  tone(10, 1976);
  delay(200);
  
  tone(10, 1319);
  delay(100);
  
  tone(10, 1760);
  delay(100);
  
  noTone(10);
  delay(200);
  
  tone(10, 1480);
  delay(200);
  
  noTone(10);
}

