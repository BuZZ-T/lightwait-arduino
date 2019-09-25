#define RED_PIN 9
#define GREEN_PIN 10
#define BLUE_PIN 11

#define INPUT_MAX_VALUE 255
#define OUTPUT_MAX_VALUE 1023
#define BLINK_INTERVAL 600

int *colors = 0;
int numberOfColors = 0;
int currentColor = 0;

//////////////////////////
#define IS_COMMON_ANODE false
const int zeroValue = IS_COMMON_ANODE ? OUTPUT_MAX_VALUE : 0;
//////////////////////////

bool isBlinking = false;

int startChar = 0;
unsigned long lastBlink = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  // write the ready signal
  Serial.write("r");

  analogWrite(RED_PIN, zeroValue);
  analogWrite(GREEN_PIN, zeroValue);
  analogWrite(BLUE_PIN, zeroValue);
}

void setColor(int index)
{
  analogWrite(RED_PIN, colors[index + 0]);
  analogWrite(GREEN_PIN, colors[index + 1]);
  analogWrite(BLUE_PIN, colors[index + 2]);
}

/**
 * Parses one color of the format "a:b:c" with a, b and c between 0 and 255.
 * Stores the result in the passed int[3] array between 0 and OUTPUT_MAX_VALUE (1023)
 */
void parseColor(String colorString, int *buf)
{
  int separatorIndex = colorString.indexOf(':');
  int separatorIndex2 = colorString.indexOf(':', separatorIndex + 1);

  String rString = colorString.substring(0, separatorIndex);
  String gString = colorString.substring(separatorIndex + 1, separatorIndex2);
  String bString = colorString.substring(separatorIndex2 + 1);

  int rInt = constrain(rString.toInt(), 0, INPUT_MAX_VALUE);
  int gInt = constrain(gString.toInt(), 0, INPUT_MAX_VALUE);
  int bInt = constrain(bString.toInt(), 0, INPUT_MAX_VALUE);

#if IS_COMMON_ANODE
  buf[0] = map(rInt, 0, INPUT_MAX_VALUE, OUTPUT_MAX_VALUE, 0);
  buf[1] = map(gInt, 0, INPUT_MAX_VALUE, OUTPUT_MAX_VALUE, 0);
  buf[2] = map(bInt, 0, INPUT_MAX_VALUE, OUTPUT_MAX_VALUE, 0);
#else
  buf[0] = map(rInt, 0, INPUT_MAX_VALUE, 0, OUTPUT_MAX_VALUE);
  buf[1] = map(gInt, 0, INPUT_MAX_VALUE, 0, OUTPUT_MAX_VALUE);
  buf[2] = map(bInt, 0, INPUT_MAX_VALUE, 0, OUTPUT_MAX_VALUE);
#endif
}

/**
 * Counts the amount of colors deliverd with the serial String.
 * Does this by counting the number of | delimiters
 */
int countColors(String colorString)
{
  int lastIndex = 0;

  int count = 1;

  while (lastIndex > -1)
  {
    lastIndex = colorString.indexOf("|", lastIndex + 1);
    if (lastIndex > -1)
    {
      count++;
    }
  }

  return count;
}

/**
 * Read new colors from the Serial connection.
 * Call this only when data is available!
 */
void readNewColors()
{
  String inn = Serial.readString();

  isBlinking = inn.charAt(0) == 'b';

  String innWithoutBlinking = isBlinking
                                  ? inn.substring(1)
                                  : inn;

  int inputLength = innWithoutBlinking.length() + 1;
  char inputChars[inputLength];
  innWithoutBlinking.toCharArray(inputChars, inputLength);

  numberOfColors = countColors(innWithoutBlinking);

  // if blinking with one color, add a second color 0:0:0
  bool addBlack = false;
  if (isBlinking && numberOfColors == 1)
  {
    addBlack = true;
    numberOfColors = 2;
  }

  unsigned long colorArraySize = numberOfColors * 3 * sizeof(int);

  // ptr of int array
  if (colors != 0)
  {
    colors = (int *)realloc(colors, colorArraySize);
  }
  else
  {
    colors = (int *)malloc(colorArraySize);
  }

  char *separator = strtok(inputChars, "|");

  int colorIndex = 0;
  while (separator != NULL)
  {
    int colorBuf[3];

    parseColor(String(separator), colorBuf);

    int colorOffset = colorIndex * 3;

    colors[colorOffset + 0] = colorBuf[0];
    colors[colorOffset + 1] = colorBuf[1];
    colors[colorOffset + 2] = colorBuf[2];

    colorIndex++;

    separator = strtok(NULL, "|");
  }

  if (addBlack)
  {
    colors[3] = zeroValue;
    colors[4] = zeroValue;
    colors[5] = zeroValue;
  }

  currentColor = 0;
  setColor(currentColor);
}

void loop()
{
  while (Serial.available() > 0)
  {
    readNewColors();
  }

  if (isBlinking)
  {
    unsigned long currentMillis = millis();

    if ((unsigned long)(currentMillis - lastBlink) >= BLINK_INTERVAL)
    {
      currentColor = (currentColor + 1) % numberOfColors;
      setColor(currentColor * 3);

      lastBlink = currentMillis;
    }
  }
}
