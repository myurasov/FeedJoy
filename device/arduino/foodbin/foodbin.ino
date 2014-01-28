#include <stdio.h>
#include <SoftwareSerial.h>

// weight stuff
const float loadA = 0; // kg
const float analogvalA = 86.60; // analog reading taken with load A on the load cell
const float loadB = 0.270; // kg
const float analogvalB = 34.07; // analog reading taken with load B on the load cell
const int timeBetweenReadings = 1000; 

float analogValueAverage = 0;
long time = 0;

// eimp
SoftwareSerial imps(9 /* tx */, 10);
SoftwareSerial s7s(7 /* tx */, 8);

void setup() {

  Serial.begin(9600);
  imps.begin(9600);

  initDisplay();

  Serial.println("==");
}

void loop() {
  int analogValue = analogRead(5);
  analogValueAverage = 0.99 * analogValueAverage + 0.01 * analogValue;

  if(millis() > time + timeBetweenReadings){
    float load = analogToLoad(analogValueAverage);

    Serial.print("ADC reading: ");Serial.println(analogValueAverage);    
    Serial.print("Load: ");Serial.println(load, 5);
    Serial.println("");

    //
    
    int grams = load * 1000;
    unsigned char loadLevel;

    // display
    // 268
    // 310
    // 331
    if (grams < 100) {
      s7s.print("    ");
      loadLevel = 0;
    } else if (grams < 268 + 20)  {
      s7s.print("8   ");
      loadLevel = 25;
    } else if (grams < 310 + 15)  {
      s7s.print("88  ");
      loadLevel = 50;
    } else if (grams < 331 + 10)  {
      s7s.print("888 ");
      loadLevel = 75;
    } else  {
      s7s.print("8888");
      loadLevel = 100;
    }

    //

    char buffer[128];
    sprintf(buffer, "{\"weight\":%d,\"loadLevel\":%d}\n", grams, loadLevel);
    imps.write(buffer);

    
    //

    time = millis();
  }
}

float analogToLoad(float analogval){
  float load = mapfloat(analogval, analogvalA, analogvalB, loadA, loadB);
  return load;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void initDisplay()
{
  s7s.begin(9600);
  clearDisplay();
  setBrightness(255);
  setDecimals(0b00000000);
  
  int d = 166;

  s7s.print("   -");
  delay(d);
  s7s.print("  --");
  delay(d);
  s7s.print(" ---");
  delay(d);
  s7s.print("----");
  delay(d);
  s7s.print("    ");
  delay(d);
  s7s.print("0000");
}

void clearDisplay()
{
  s7s.write(0x76);
}

void setBrightness(byte value)
{
  s7s.write(0x7A);
  s7s.write(value);
}

void setDecimals(byte decimals)
{
  s7s.write(0x77);
  s7s.write(decimals);
}
