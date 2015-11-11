
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>
#include <NewPing.h>

#define TRIGGER_PIN  6 
#define ECHO_PIN     5  
#define TRIGGER_PINS  9 
#define ECHO_PINS     3 
#define TRIGGER_PIN_THREE     7 
#define ECHO_PIN_THREE     4 
#define MAX_DISTANCE 200

SoftwareSerial mySerial(8, 7);
const int chipSelect = 10;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing second_sonar(TRIGGER_PINS, ECHO_PINS, MAX_DISTANCE);
NewPing third_sonar(TRIGGER_PIN_THREE, ECHO_PIN_THREE, MAX_DISTANCE);
Adafruit_GPS GPS(&mySerial);

#define GPSECHO  true

boolean usingInterrupt = false;
void useInterrupt(boolean); 
int total;
void setup()  
{

  Serial.begin(115200);
  Serial.println("Adafruit GPS library basic test!");
  
  GPS.begin(9600);
  

  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   
  GPS.sendCommand(PGCMD_ANTENNA);

  useInterrupt(false);
  pinMode(10, OUTPUT);
    SD.begin(chipSelect);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);
  mySerial.flush();
  mySerial.end();
}

SIGNAL(TIMER0_COMPA_vect) {
  char c = GPS.read();
#ifdef UDR0
  if (GPSECHO)
    if (c) UDR0 = c;   
#endif
}

void useInterrupt(boolean v) {
  if (v) {

    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {

    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}

uint32_t timer = millis();

void loop()
{
  int final = 0; 
  int another=0; //sonar sensor part
  int myInts[20];
  int moreInts[20];
  delay(50);
  for (int pin = 0; pin < 20; pin++) {
  unsigned int first = sonar.ping();
  myInts[pin]=first;
  delay(100);
  unsigned int sec = second_sonar.ping();
  moreInts[pin]=sec;
  delay(100);
  }
  for (int i=0; i<20;i++){
    if (myInts[i] > 1){
        total = total + myInts[i];
        final = final + 1;
        another = another+moreInts[i];
    }

  
  }
  total = total/final;
  another = another/final;
  if (total < 0){
  total = total*-1;
  }
  if (another < 0){
  another = another*-1;
  }

  GPS.begin(9600);
  uint32_t start_time = millis();
  do {

    GPS.read();
    if (GPS.newNMEAreceived())
      GPS.parse( GPS.lastNMEA() );

  } while (millis() - start_time < 2000);
    mySerial.flush();
mySerial.end();
delay(100);
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
    dataFile.print(total);
    dataFile.print(" / ");
    dataFile.print(another);
    dataFile.print(" / ");
    dataFile.print(third_sonar.ping());
    dataFile.print(" : ");
    dataFile.print(GPS.altitude);
    dataFile.print(" ; ");
    dataFile.print(GPS.hour, DEC); 
    dataFile.print(".");
    dataFile.print(GPS.minute, DEC);
    dataFile.print(".");
    dataFile.println(GPS.seconds, DEC);
    dataFile.close();

  
}
