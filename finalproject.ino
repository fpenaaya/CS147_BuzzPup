// This #include statement was automatically added by the Particle IDE.
#include <Grove_LCD_RGB_Backlight.h>

// This #include statement was automatically added by the Particle IDE.
#include <Ubidots.h>

#ifndef TOKEN
#define TOKEN "BBFF-YdP0uXqQ6MPws5pNS62Ad1eM0ZJa6x"
#endif

Ubidots ubidots(TOKEN, UBI_TCP);
#define BUZZER_PIN 6

int pirPin = 2;                 // PIR Out pin 
int pirStat = 0;  

rgb_lcd lcd;

const int colorR = 89;
const int colorG = 77;
const int colorB = 255;

void setup() {
     Serial.begin(9600);
     
     pinMode(pirPin, INPUT);     
     pinMode(BUZZER_PIN, OUTPUT);
     
     lcd.begin(16, 2);
     lcd.setRGB(colorR, colorG, colorB);
     lcd.print("INITIALIZING...");
}

void loop() {
    pirStat = digitalRead(pirPin); 
    if (pirStat == HIGH) {            // if motion detected
    
       if (checkMovement())
       {
            lcd.clear();
            lcd.print("STATUS: INTRUDER");
            digitalWrite(BUZZER_PIN, HIGH);
            ubidots.add("Status", 1);
            Serial.println("state: one");
            
            bool bufferSent = false;
            bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id
            
            if(bufferSent){
            // Do something if values were sent properly
            Serial.println("Values sent by the device");
            }
    
       }
       
    }
    else
    {
        lcd.clear();
        lcd.print("STATUS: OK");
        digitalWrite(BUZZER_PIN, LOW);
        Serial.println("state: zero");
    }
    
    delay(5000);
}

bool checkMovement() {
    int toStop = millis() + 5000;
    int curr = millis();
    int zeroCounter = 0;
    int oneCounter = 0;
    while(curr < toStop)
    {
        if(digitalRead(pirPin) == 0)
        {
            zeroCounter++;
        }
        else
        {
            oneCounter++;
        }
        curr = millis();
    }
    if (oneCounter > zeroCounter*0.5)
    {
        Serial.println("ring alarm: true");
        return true;
        
    }
    //else
    Serial.println("ring alarm: false");
    return false;
}

