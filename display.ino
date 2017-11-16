/*  draw text's APP
    drawChar(INT8U ascii,INT16U poX, INT16U poY,INT16U size, INT16U fgcolor);
    drawString(char *string,INT16U poX, INT16U poY,INT16U size,INT16U fgcolor);
*/

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
#include <TimeLib.h>
//#include <strings.h>

#define X_POS_TIME 0
#define Y_POS_TIME 0
#define X_POS_POWER 0
#define Y_POS_POWER 20
#define X_POS_CONNECTSTATUS 0
#define Y_POS_CONNECTSTATUS 300
#define X_STATUS_OFFSET 180
#define BACKGROUND_COLOR BLACK
#define SUCCESS_COLOR GREEN
#define ERR_COLOR RED
#define PRIMARY_TEXT_COLOR BLUE


void setup(){
    TFT_BL_ON;      // turn on the background light
    Tft.TFTinit();  // init TFT library

    Tft.drawString("Initialized!", 0, 0, 1, GREEN);
}

long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

// clear the entire screen... consider drawing a small rectangle over the area to be updated
void clearScrn(){
  Tft.fillRectangle(0, 0, 239, 349, BACKGROUND_COLOR);
}

 
void setupForTime(){
  // clearScrn();
  Tft.drawString("Uptime:", X_POS_TIME, Y_POS_TIME, 2, PRIMARY_TEXT_COLOR);
  Tft.drawString(":", 130, Y_POS_TIME, 2, PRIMARY_TEXT_COLOR);
  Tft.drawString(":", 170, Y_POS_TIME, 2, PRIMARY_TEXT_COLOR);
}

void updateTime(){
  Tft.fillRectangle(100, 0, 30, 20, BACKGROUND_COLOR);
  Tft.fillRectangle(140, 0, 30, 20, BACKGROUND_COLOR);
  Tft.fillRectangle(180, 0, 30, 20, BACKGROUND_COLOR);
  Tft.drawNumber(hour(), 100, 0, 2, PRIMARY_TEXT_COLOR);
  Tft.drawNumber(minute(), 140, 0, 2, PRIMARY_TEXT_COLOR);
  Tft.drawNumber(second(), 180, 0, 2, PRIMARY_TEXT_COLOR);
}

void setupForVoltage(){
  Tft.drawString("Power:", X_POS_POWER, Y_POS_POWER, 2, PRIMARY_TEXT_COLOR);
}

void updateVoltage(){
  Tft.fillRectangle(80, Y_POS_POWER, 50, 20, BACKGROUND_COLOR);
  Tft.drawNumber(readVcc(), 80, Y_POS_POWER, 2, PRIMARY_TEXT_COLOR);
}

void setupForStatus(){
  Tft.drawString("Sys status:", X_POS_CONNECTSTATUS, Y_POS_CONNECTSTATUS, 2, PRIMARY_TEXT_COLOR);
}

int getStatus(){ // not yet implemented
  return 1;
}

void updateStatus(){
  int stat = getStatus();
  Tft.fillRectangle(X_POS_CONNECTSTATUS+X_STATUS_OFFSET, Y_POS_CONNECTSTATUS, 50, 20, BACKGROUND_COLOR);
  if (stat){
    Tft.drawString("[OK]", X_POS_CONNECTSTATUS+X_STATUS_OFFSET, Y_POS_CONNECTSTATUS, 2, SUCCESS_COLOR);
  }
  else{
    Tft.drawString("[ERR]", X_POS_CONNECTSTATUS+X_STATUS_OFFSET, Y_POS_CONNECTSTATUS, 2, ERR_COLOR);
  }
  
}

void loop(){
//  hiDiana();
  clearScrn();
  setupForTime();
  setupForVoltage();
  setupForStatus();
  int i=0;
  while(true){
    updateTime();
    updateVoltage();
    updateStatus();
    delay(100);
  }
}
