#include <Arduino.h>
/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-temperature-sensor-oled
 * 
 */

// ************ Neo Pixels **************

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            9

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      11

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 170; // delay for half a second






#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <LiquidCrystal_I2C.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SENSOR_PIN A2 // Arduino pin connected to DS18B20 sensor's DQ pin

Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); // create SSD1306 display object connected to I2C
OneWire oneWire(SENSOR_PIN);                                   // setup a oneWire instance
DallasTemperature tempSensor(&oneWire);                        // pass oneWire to DallasTemperature library
LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14, POSITIVE);

String tempString;
void oledDisplayCenter(String text);

//Variables
int chk;
int LED = 8;
int Piezo = A0;


void setup()
{
    Serial.begin(115200);
    pixels.begin(); // This initializes the NeoPixel library.

  while (lcd.begin(16, 2) != 1) //colums - 20, rows - 4
  {
    Serial.println(F("PCF8574 is not connected or lcd pins declaration is wrong. Only pins numbers: 4,5,6,16,11,12,13,14 are legal."));
    delay(5000);   
  }
    //   Serial.begin(9600);
    lcd.print(F("PCF8574 is OK...")); //(F()) saves string to flash & keeps dynamic memory free
    delay(2000);

    lcd.clear();

    // initialize OLED display with address 0x3C for 128x64
    if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        Serial.println(F("SSD1306 allocation failed"));
        while (true)
            ;
    }

    delay(2000);         // wait for initializing
    oled.clearDisplay(); // clear display

    oled.setTextSize(1); // text size
    oled.setFont(&FreeSansBoldOblique9pt7b);
    oled.setTextColor(WHITE); // text color
    oled.setCursor(0, 10);    // position to display

    tempSensor.begin();     // initialize the sensor
    tempString.reserve(10); // to avoid fragmenting memory when using String
}

void loop()
{
    tempSensor.requestTemperatures();                  // send the command to get temperatures
    float tempCelsius = tempSensor.getTempCByIndex(0); // read temperature in Celsius
    lcd.clear();
    lcd.print(F("Temperate:"));
    lcd.setCursor(11,0);
    lcd.print((float)tempSensor.getTempCByIndex(0));
    delay(2000);
    // lcd.clear();
    
    tempString = "T: ";
    tempString += String(tempCelsius, 2); // two decimal places
    tempString += " C";

    Serial.println(tempString); // print the temperature in Celsius to Serial Monitor
    oledDisplayCenter(tempString);

    // **************** Neopixel *****************
    
  if ((tempCelsius >= 22) && (tempCelsius < 26)) {

    for(int i=0;i<NUMPIXELS;i++){
  
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(10,100,23)); // Moderately bright green color.
  
      pixels.show(); // This sends the updated pixel color to the hardware.
  
      delay(delayval); // Delay for a period of time (in milliseconds).
  
    }
     
  }
  
else if (tempCelsius >= 26) {
   
    for(int i=0;i<NUMPIXELS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(175,10,58)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).



  }}



   
}

void oledDisplayCenter(String text)
{
    int16_t x1;
    int16_t y1;
    uint16_t width;
    uint16_t height;

    oled.getTextBounds(text, 0, 0, &x1, &y1, &width, &height);

    // display on horizontal and vertical center
    oled.clearDisplay(); // clear display
    oled.setCursor((SCREEN_WIDTH - width) / 2, (SCREEN_HEIGHT - height) / 2);
    oled.println(text); // text to display
    oled.display();
    delay(200);

    
}