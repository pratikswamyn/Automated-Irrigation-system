#include <RTClib.h>
#include <EEPROM.h>
#include <TM1637Display.h>

#define EEPROM_ADDR_ONHOURS 0
#define EEPROM_ADDR_ONMINUTES 1
#define EEPROM_ADDR_OFFHOURS 2
#define EEPROM_ADDR_OFFMINUTES 3
#define EEPROM_ADDR_CURRENT_HOURS 4
#define EEPROM_ADDR_CURRENT_MINUTES 5

const int onhoursSwitchPin = 4;
const int onminutesSwitchPin = 5;
const int relayPin = 12;
const int buttonPin = 7;

int storedOnhours = 0;
int storedOnminutes = 0;
int storedOffhours = 0;
int storedOffminutes = 0;

int onhours = 0;
int onminutes = 0;
int offhours = 0;
int offminutes = 0;

RTC_DS3231 rtc;

TM1637Display display(2, 3);

int displayState = 0;

void updateDisplay();
void switchDisplayState();
void updateCurrentTime();

void setup() {
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    Wire.begin();
    rtc.begin();
    pinMode(relayPin, OUTPUT);
    digitalWrite(relayPin, LOW);
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    pinMode(onhoursSwitchPin, INPUT_PULLUP);
    pinMode(onminutesSwitchPin, INPUT_PULLUP);
    pinMode(buttonPin, INPUT_PULLUP);
    
    storedOnhours = EEPROM.read(EEPROM_ADDR_ONHOURS);
    storedOnminutes = EEPROM.read(EEPROM_ADDR_ONMINUTES);
    storedOffhours = EEPROM.read(EEPROM_ADDR_OFFHOURS);
    storedOffminutes = EEPROM.read(EEPROM_ADDR_OFFMINUTES);
    
    if (storedOnhours >= 0 && storedOnhours < 24) {
        onhours = storedOnhours;
    }
    if (storedOnminutes >= 0 && storedOnminutes < 60) {
        onminutes = storedOnminutes;
    }
    if (storedOffhours >= 0 && storedOffhours < 24) {
        offhours = storedOffhours;
    }
    if (storedOffminutes >= 0 && storedOffminutes < 60) {
        offminutes = storedOffminutes;
    }
    
    display.setBrightness(0);
    
    updateDisplay();
}

void loop() {
    DateTime now = rtc.now();
    int minutes = now.minute();
    int hours = now.hour();
    int currentTimeMinutes = (hours * 60) + minutes;
    String formattedMinutes = (minutes < 10) ? "0" + String(minutes) : String(minutes);
    String formattedHours = (hours < 10) ? "0" + String(hours) : String(hours);
    delay(10);

       switch (displayState) {

    case 0:

        digitalWrite(8, HIGH);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);

        if (digitalRead(onhoursSwitchPin) == LOW) {
                int currentHours = rtc.now().hour();
                currentHours = (currentHours + 1) % 24;
                rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), currentHours, rtc.now().minute(), 0));
                updateCurrentTime();
                delay(100);
            }
        if (digitalRead(onminutesSwitchPin) == LOW) {
                int currentMinutes = rtc.now().minute();
                currentMinutes = (currentMinutes + 1) % 60;
                rtc.adjust(DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), rtc.now().hour(), currentMinutes, 0));
                updateCurrentTime();
                delay(100);
            }
        break;

    case 1:

        digitalWrite(9, HIGH);
        digitalWrite(10, LOW);
        digitalWrite(8, LOW);
    
        if (digitalRead(onhoursSwitchPin) == LOW) {
            onhours = (onhours + 1) % 24;
            updateDisplay();
            delay(100); 
        }
        if (digitalRead(onminutesSwitchPin) == LOW) {
            onminutes = (onminutes + 1) % 60;
            updateDisplay();
            delay(100); 
        }

        break;

    case 2:
        
        digitalWrite(10, HIGH);
        digitalWrite(9, LOW);
        digitalWrite(8, LOW);

        if (digitalRead(onhoursSwitchPin) == LOW) {
            offhours = (offhours + 1) % 24;
            updateDisplay();
            delay(100); 
        }
        if (digitalRead(onminutesSwitchPin) == LOW) {
            offminutes = (offminutes + 1) % 60;
            updateDisplay();
            delay(100);
        }
      break;

      
       
       }

  if (storedOnhours != onhours) {
        storedOnhours = onhours;
        EEPROM.write(EEPROM_ADDR_ONHOURS, storedOnhours);
     }
    delay(10);
  if (storedOffhours != offhours) {
        storedOffhours = offhours;
        EEPROM.write(EEPROM_ADDR_OFFHOURS, storedOffhours);
     }
     delay(10);
  if (storedOnminutes != onminutes) {
        storedOnminutes = onminutes;
        EEPROM.write(EEPROM_ADDR_ONMINUTES, storedOnminutes);
     }
     delay(10);
  if (storedOffminutes != offminutes) {
        storedOffminutes = offminutes;
        EEPROM.write(EEPROM_ADDR_OFFMINUTES, storedOffminutes);
     }
     delay(10);
    
    int onTimeMinutes = (onhours * 60) + onminutes ;
    int offTimeMinutes = (offhours * 60) + offminutes ;
    
    
    
    if (offTimeMinutes < onTimeMinutes) {
    if (currentTimeMinutes >= onTimeMinutes || currentTimeMinutes < offTimeMinutes) {
      digitalWrite(relayPin, HIGH); 
    }
    else {
      digitalWrite(relayPin, LOW); 
    }
  } 
  else {
    if (currentTimeMinutes >= onTimeMinutes && currentTimeMinutes < offTimeMinutes) {
      digitalWrite(relayPin, HIGH); 
    } 
    else {
      digitalWrite(relayPin, LOW);  
    }
  }
    
  delay(10);

    
    if (digitalRead(buttonPin) == LOW) { 
        switchDisplayState();
        delay(200); 
    }

     updateDisplay();
    
}

void updateDisplay() {
    int displayHours, displayMinutes;

    switch (displayState) {
        case 0: 
               {
                DateTime now = rtc.now();
                int minutes = now.minute();
                int hours = now.hour();
                displayHours = hours;
                displayMinutes = minutes;
                
               }
            break;

        case 1: 
            displayHours = onhours;
            displayMinutes = onminutes;
            break;

        case 2: 
            displayHours = offhours;
            displayMinutes = offminutes;
            break;

         
    }

    display.showNumberDecEx(displayHours * 100 + displayMinutes, 0b11100000, true);
}

void updateCurrentTime() {
    int currentHours = rtc.now().hour();
    int currentMinutes = rtc.now().minute();

    EEPROM.write(EEPROM_ADDR_CURRENT_HOURS, currentHours);
    EEPROM.write(EEPROM_ADDR_CURRENT_MINUTES, currentMinutes);

    display.showNumberDecEx(currentHours * 100 + currentMinutes, 0b11100000, true);
}

void switchDisplayState() {
    displayState = (displayState + 1) % 3; 
    updateDisplay();
}
