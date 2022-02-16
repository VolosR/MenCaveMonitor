#include <esp_now.h>
#include <WiFi.h>
#include <TFT_eSPI.h> 
#include "BAT.h"
#include "font.h"

#define gray 0xBDD7
#define yel 0xFEC0

TFT_eSPI tft = TFT_eSPI(); 

const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 5;

typedef struct struct_message {
  int h;
  int m;
  int s;
  int wd;
  int mo;
  int da;
  int ye;

  float tmpC;
  float tmpF;
  float pre;
  float hum;
  
} struct_message;


struct_message myData;
String minutes, seconds, hours,days,months;

String Wdays[7]={"MON","TUE","WED","THU","FRI","SAT","SUN"};
bool metric=1;

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  if(myData.m<10)
  minutes="0"+String(myData.m);
  else
  minutes=String(myData.m);

    if(myData.s<10)
  seconds="0"+String(myData.s);
  else
  seconds=String(myData.s);

  if(myData.h<10)
  hours="0"+String(myData.h);
  else
  hours=String(myData.h);

  if(myData.da<10)
  days="0"+String(myData.da);
  else
  days=String(myData.da);

    if(myData.mo<10)
  months="0"+String(myData.mo);
  else
  months=String(myData.mo);
  
  tft.setTextColor(yel,TFT_BLACK);
  tft.drawString(Wdays[myData.wd-1],172,88,2);
  tft.drawString(days+"/"+months,172,104,2);
  tft.drawString("YoutubeChannel",92,214,2);
  tft.setFreeFont(&DSEG14_Classic_Bold_18);
  tft.setTextColor(gray,TFT_BLACK); 
  tft.drawString(seconds,132,100);
  
  
  tft.setFreeFont(&DSEG7_Classic_Bold_30);
  tft.setTextColor(gray,TFT_BLACK);
  tft.drawString(hours+":"+minutes,18,88);
  tft.setFreeFont(&DSEG7_Classic_Bold_36);
  if(metric==1)
  tft.drawString(String((int)myData.tmpC),12,166 );
  else
  tft.drawString(String((int)myData.tmpF),12,166 );
  
  tft.drawString(String((int)myData.hum)+"%",100,180,4);
  tft.drawString(String((int)myData.pre)+"hPa",174,176,2);

    tft.drawCircle(76,162,3,gray);
}


void setup() {

    pinMode(12,INPUT_PULLUP);
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

  
  
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(17, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, 120);
  
  tft.init();
  tft.setRotation(0);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0,0,240,240,BAT);
  
}

int pom=0;

void loop() {

if(digitalRead(12)==0)
{
   if(pom==0)
   {
    pom=1;
    metric=!metric;
    delay(200);
    }
  
}else pom=0;

}
