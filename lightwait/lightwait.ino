#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#define OUTPUT_MAX_VALUE 1023
#define BLINK_INTERVAL 600

int red = 0;
int blue = 0;
int green = 0;

//////////////////////////
#define IS_COMMON_ANODE true
const int zeroValue = IS_COMMON_ANODE ? HIGH : LOW;
//////////////////////////

bool isBlinking = false;

int startChar = 0;
bool isOn = false;
unsigned long lastBlink = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  // write the ready signal
  Serial.write("r");

  digitalWrite(RED_PIN, zeroValue);
  digitalWrite(GREEN_PIN, zeroValue);
  digitalWrite(BLUE_PIN, zeroValue);
}

void setNewColor()
{
  String inn = Serial.readString();

  if (inn.charAt(0) == 'b')
  {
    isBlinking = true;
    startChar = 1;
  }
  else
  {
    isBlinking = false;
    startChar = 0;
  }

  int separatorIndex = inn.indexOf(':');
  int separatorIndex2 = inn.indexOf(':', separatorIndex + 1);

  String rString = inn.substring(startChar, separatorIndex);
  String gString = inn.substring(separatorIndex + 1, separatorIndex2);
  String bString = inn.substring(separatorIndex2 + 1);

  red = map(rString.toInt(), 0, 255, 0, OUTPUT_MAX_VALUE);
  green = map(gString.toInt(), 0, 255, 0, OUTPUT_MAX_VALUE);
  blue = map(bString.toInt(), 0, 255, 0, OUTPUT_MAX_VALUE);

  if (IS_COMMON_ANODE)
  {
    red = map(red, 0, OUTPUT_MAX_VALUE, OUTPUT_MAX_VALUE, 0);
    green = map(green, 0, OUTPUT_MAX_VALUE, OUTPUT_MAX_VALUE, 0);
    blue = map(blue, 0, OUTPUT_MAX_VALUE, OUTPUT_MAX_VALUE, 0);
  }

  digitalWrite(RED_PIN, red);
  digitalWrite(GREEN_PIN, green);
  digitalWrite(BLUE_PIN, blue);
}

void loop()
{
  while (Serial.available() > 0)
  {
    setNewColor();
  }

  if (isBlinking)
  {
    unsigned long currentMillis = millis();

    if (currentMillis - lastBlink > BLINK_INTERVAL)
    {
      if (isOn)
      {
        digitalWrite(RED_PIN, zeroValue);
        digitalWrite(GREEN_PIN, zeroValue);
        digitalWrite(BLUE_PIN, zeroValue);
      }
      else
      {
        digitalWrite(RED_PIN, red);
        digitalWrite(GREEN_PIN, green);
        digitalWrite(BLUE_PIN, blue);
      }
      isOn = !isOn;
      lastBlink = currentMillis;
    }
  }
}
