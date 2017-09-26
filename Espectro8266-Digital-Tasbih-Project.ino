
/* Program Digital Tasbih

Created 26 Sept 2017
by Vica Rizqia Hidayat

*/

 
// Include the correct display library
// For a connection via I2C using Wire include
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
#include "Adafruit_MPR121.h"

// Initialize the OLED display using Wire library
SSD1306  display(0x3c, 4, 5);


#define DEMO_DURATION 20000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

int hitung;
char b[10];
char counter2;
String str;

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
   // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  
 

   while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  //Default address is 0x5A, if tied to 3.3V its 0x5B
   //If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.print("MPR121 found!");

for(int i=0; i<499 ; i++){
  display.clear();
  drawProgressBarDemo();
  display.display();
  counter++;
  delay(10);
  }

display.clear();
drawDigitalTasbih();
display.display();
delay(2000);
display.clear();
drawHitunganTasbih();
display.display();
delay(2000);
display.clear();
drawTextHitungan();
display.display();

}

void drawDigitalTasbih() {
    // Font Demo1
    // create more fonts at http://oleddisplay.squix.ch/
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64,22, "DIGITAL TASBIH");  
    
}

void drawHitunganTasbih() {
    display.setFont(ArialMT_Plain_10);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawStringMaxWidth(64,22,128,"HITUNGAN TASBIH" );
    delay(1000);
}

void drawTextHitungan(){
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawStringMaxWidth(64,22,128, "KE");
  
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
  
}

void drawTextB(){
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawStringMaxWidth(64,22,128, (b));
   
}

Demo demos[] = {drawProgressBarDemo, drawDigitalTasbih, drawHitunganTasbih, drawTextHitungan,drawTextB };
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;


void loop() {
 
 currtouched = cap.touched();
  
  for (uint8_t i=0; i<12; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
     hitung++;
      //Serial.print(counter); Serial.println();
      str=String(hitung);
      str.toCharArray(b,10);
      display.clear();
      drawTextB();
      display.display();

       // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
     }
    }
  }
  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
  
  // put a delay so it isn't overwhelming
  delay(100);
  }
