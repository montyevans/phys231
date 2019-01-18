// Install this from Tools -> Manage Libraries. Handy if you don't want a thousand lines of code.
#include <ArduinoSTL.h>

using namespace std;

int timestep = 500;
int counter = 0;
int frequency = 60;

vector<int> pins = {11, 12, 13};
vector<vector<char> > permutations = {{'Z','L', 'H'}, {'Z','H', 'L'}, {'L','Z', 'H'}, {'L','H', 'Z'}, {'H','Z', 'L'}, {'H','L', 'Z'}};

// For some reason I have to forward declare this, and only this, function. Hey ho.
vector<int> highBits(int value);

void setup() {
  Serial.begin(9600);
  Serial.println("Initialized");
}

void loop() {
  fireMultipleLeds(highBits(counter % 64));
  counter++;
}

// Get the index of the bits of a given number that are high. Ex: highBits(6) will return {0, 2}, because precisely the 0th & 2nd bits are high.
vector<int> highBits(int value) {
  vector<int> highBits;
  int bitNum = 0;
  
  while (true) {
    if (value == 0) { break; }
    if (value % 2) { highBits.push_back(bitNum); }
    value /= 2;
    bitNum++;
  }
  
  return highBits;
}

void fireMultipleLeds(vector<int> ledNums) {
  if (ledNums.size() == 0) {
    for (int i = 0; i < pins.size(); i++) {
      pinMode(pins[i], INPUT);
    }
    delay(timestep);
    return;
  }
  
  int totalDelay = 0;
  double cycleTime = 1000 / frequency; // The time between flashes of the same LED
  double flashTime = cycleTime / ledNums.size(); // Time between flashes of each LED

  while (true) {
    for (int i = 0; i < ledNums.size(); i++) {
      fireLed(ledNums[i]);
      delay(flashTime);
      totalDelay += flashTime;
    }
    if (totalDelay >= timestep) {
      break;
    }
  }
}

// Each of the permutations of (H, L, Z) turns one of the LEDs on / off.
void fireLed(int ledNum) {
  vector<char> thisPerm = permutations[ledNum];
  for (int i = 0; i < thisPerm.size() ; i++) {
    char thisChar = thisPerm[i];
    switch (thisChar){
    case 'Z':
      pinMode(pins[i], INPUT);
      break;
    case 'L':
      pinMode(pins[i], OUTPUT);
      digitalWrite(pins[i], LOW);
      break;
    case 'H':
      pinMode(pins[i], OUTPUT);
      digitalWrite(pins[i], HIGH);
      break;
    }
  }
}
