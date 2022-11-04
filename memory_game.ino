
const int redButton = 5;
const int blueButton = 4;
const int greenButton = 3;
const int yellowButton = 2;
const int redLED = 10;
const int blueLED = 9;
const int greenLED = 8;
const int yellowLED = 7;
const int buzzer = 12;

const int tones[] = {1900, 1600, 1300, 1000, 3200};   //different buzzer noises
int buttonState[] = {0, 0, 0, 0};                     //current state of button
int lastButtonState[] = {0, 0, 0, 0};                 //previous state of button
int buttonCounter[] = {0, 0, 0, 0};                   //is a button pressed or not
int gameOn = 0;                                       //new game starts when = 0
int wait = 0;                                         //wait for a pattern input
int currentLevel = 1;                                //current level and number of button presses for next level
int dlay = 500;                                      //wait .5 seconds in between presses (no accidental double clicks)
int ledTime = 500;                                    //how long LEDs flash (.5 seconds)
int n_levels = 10;                                    //number of levels until you win then it gets faster
int pinAndTone = 0;                                   //determines which LED and buzzer tone/noise
int correct = 0;                                      //must change to 1 to move on to next level. Are you right or not
int speedFactor = 5;                                  //speed of game. increases when you win
int ledDelay = 200;                                   //time in between each LED. (.2 but decreases when you win) 

void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2)
    {
      digitalWrite(buzzer, HIGH);                     //turns on buzzer
      delayMicroseconds(tone);                        //creates tone/noise of it
      digitalWrite(buzzer, LOW);                      //turns it off
      delayMicroseconds(tone);
    }
}

void setup() {
  randomSeed(analogRead(0));                          //generate random number
  
  pinMode(redButton, INPUT);                          //all buttons are inputs because we have to click/input to activate
  pinMode(blueButton, INPUT);
  pinMode(greenButton, INPUT);
  pinMode(yellowButton, INPUT);

  pinMode(redLED, OUTPUT);                            //all LEDs are outputs because they output for us to see
  pinMode(blueLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  }
  
void loop() {
  int n_array[n_levels];                              //will store randomized game pattern
  int u_array[n_levels];                              //stores user inputs
  int i;
  if (gameOn == 0){                                   //only if a new game/level
    for (i = 0; i < n_levels; i = i + 1){                //saves the number in n_array to generate random pattern
      n_array[i] = 0;
      u_array[i] = 0;
      n_array[i] = random(0,4);
    }
    gameOn = 1;                                       //tells game to run
  }

if (wait == 0) {
  delay (200);
  i = 0;
  for (i = 0; i < currentLevel; i = i + 1){             //shows current game pattern
    ledDelay = ledTime/(1 + (speedFactor/n_levels)*(currentLevel - 1));
    pinAndTone = n_array[i];
    digitalWrite(pinAndTone + 7, HIGH);
    playTone(tones[pinAndTone], ledDelay);
    digitalWrite(pinAndTone + 7, LOW);
    delay(100/speedFactor);
  }
  wait = 1;                                          //puts game on hold until user puts in pattern
 }
 i=0;
 int buttonChange = 0;                               //will be used to detect a button press
 int j = 0;                                          //j is current position in pattern
 while (j < currentLevel) {                          //only repeats as many times as the level you are on
  while (buttonChange == 0){
    for (i = 0; i < 4; i = i + 1){                   //determines which button is pressed
      buttonState[i] = digitalRead(i + 2);
      buttonChange += buttonState[i];
    }
  }
  for(i = 0; i < 4; i = i + 1){                      //turns on LED and buzzer noise corresponding to button pressed
    if (buttonState[i] == HIGH) {
      digitalWrite(i + 7, HIGH);
      playTone(tones[i], ledTime);
      digitalWrite(i + 7, LOW);
      wait = 0;
      u_array[j] = i;                                //stores user input to compare to game pattern
      buttonState[i] = LOW;
      buttonChange = 0;
    }
  }
  if (u_array[j] == n_array[j]){                     //checks if you put in pattern right
    j++;
    correct = 1;
  }
  else{
    correct = 0;
    i = 4;
    j = currentLevel;
    wait = 0;
  }
 }
 if(correct == 0){                                   //if wrong then reset everything
  delay(300);
  i = 0;
  gameOn = 0;
  currentLevel = 1;
  for (i = 0; i < 4; i = i + 1){                     //These for loops will make the all LEDs and buzzer go off at the same time
    digitalWrite(i+7, HIGH);
  }
  playTone(tones[4], ledTime);
  for (i = 0; i < 4; i = i + 1){
    digitalWrite(i+7, LOW);
  }
  delay(200);
  for (i = 0; i < 4; i = i + 1){
    digitalWrite(i+7, HIGH);
  }
  playTone(tones[4], ledTime);
  for (i = 0; i < 4; i = i + 1){
    digitalWrite(i+7, LOW);
  }
  delay(500);
  gameOn = 0;
 }
 if (correct == 1){                                   //if correct go up one level
  currentLevel++;
  wait = 0;
 }
 if (currentLevel == n_levels){                       //winning song if you win 10 in a row
  delay(500);
  int notes[] = {2, 2, 2, 2, 0, 1, 2, 1, 2};
  int note = 0;
  int tempo [] = {200, 200, 200, 400, 400, 400, 200, 200, 600};
  int breaks[] = {100, 100, 100, 200, 200, 200, 300, 100, 200};
  for (i = 0; i < 9; i = i + 1){
    note = notes[i];
    digitalWrite(note + 7, HIGH);
    playTone(tones[note], tempo[i]);
    digitalWrite(note + 7, LOW);
    delay(breaks[i]);
  }
  gameOn = 0;
  currentLevel = 1;                                     //start back at level 1
  n_levels = n_levels + 2;                              //adds 2 more levels to max to make more difficult since you won 10 in a row
  speedFactor += 1;                                     //increases speed factor by 1
 }
}
