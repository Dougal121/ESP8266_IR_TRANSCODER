// Board Selection LoLin(WeMos) D1 R2 & mini (using a node mcu board and DX's are lineing up OK
//
#include <ESP8266WiFi.h>
#include <WiFiUDP.h> 
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266httpUpdate.h>
#include <TimeLib.h>
#include <Wire.h>
#include <EEPROM.h>
#include <stdio.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRrecv.h>
#include <IRutils.h>

#define MYVER 0x11223344

#define DECODE_NEC 1
#define TVONOFF   0xFB38C7
#define TVAV      0xFBE817   // signal source
#define TVVOLUP   0xFBD22D
#define TVVOLDN   0xFBF20D
#define TVCHUP    0xFB906F
#define TVCHDN    0xFBA05F
#define TV1PAD    0xFB2AD5
#define TV2PAD    0xFB6897    
#define TV3PAD    0xFBA857
#define TV4PAD    0xFB0AF5
#define TV5PAD    0xFB48B7
#define TV6PAD    0xFB8877
#define TV7PAD    0xFB32CD
#define TV8PAD    0xFB708F
#define TV9PAD    0xFBB04F
#define TV0PAD    0xFB30CF
#define TVLNAV    0xFBE21D
#define TVRNAV    0xFBE01F
#define TVUNAV    0xFB58A7
#define TVDNAV    0xFB12ED
#define TVOKNAV   0xFB609F
#define TVAPMENU  0xFB18E7
#define TVEXITNAV 0xFB50AF
#define TVMUTE    0xFB10EF
#define TVPLAY    0xFB5AA5
#define TVPAUSE   0xFB4AB5
#define TVSTOP    0xFBBA45
#define TVWTF     0xFB3AC5
#define TVREW     0xFBAA55
#define TVFF      0xFB7A85
#define TVPREV    0xFB8A75
#define TVNEXT    0xFB9A65
#define TVTEXT    0xFB9A65
#define TVSUBTTL  0xFBFA05
#define TVFREEZE  0xFB9867
#define TVINFO    0xFB1AE5
#define TVRED     0xFB6A95
#define TVGREEN   0xFBEA15
#define TVYELLOW  0xFBF807
#define TVBLUE    0xFBDA25

#define PAN2NEC   0x0503   // these are a cheat for assigning in the back in the box function
#define NECONLY   0x0003
#define PANONLY   0x0005

const uint16_t kIrLed = D2;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

uint16_t RECV_PIN = D5 ;  // 14
IRrecv irrecv(RECV_PIN);

ESP8266WebServer server(80) ;
ESP8266WebServer OTAWebServer(81) ;
ESP8266HTTPUpdateServer OTAWebUpdater ;
time_t chiptime ; 

decode_results results;

#define MAX_COLS 4
#define MAX_DISPLAY 8
#define MAX_PAD 48
#define MAX_VR 3
#define BUFF_MAX 64
#define MAX_WIFI_TRY 45
char buff[BUFF_MAX]; 
const byte LED = BUILTIN_LED ;  // = 16  4 ? D4  
const byte LEDR = D6 ;          // = 16  4 ? D4  
const byte LEDG = D7 ;          // = 16  4 ? D4  
const byte LEDB = D8 ;          // = 16  4 ? D4  

//SSD1306 display(0x3c, 5, 4);   // GPIO 5 = D1, GPIO 4 = D2   - onboard display 0.96" 
//SH1106Wire display(0x3c, 4, 5);   // arse about ??? GPIO 5 = D1, GPIO 4 = D2  -- external ones 1.3"

typedef struct __attribute__((__packed__)) {             // permanent record
  IPAddress MyIP  ;               
  IPAddress MyIPC  ;  
  char nssid[32] ;
  char npassword[16] ;
  char cssid[32] ;
  char cpassword[16] ;
  char NodeName[16] ;
  long lTxDelay ;     // ms delay between rx and tx 
  int  butW ;         // button width pixels
  int  butH ;         // button heigh Pixels
  int  butC ;         // Number of columns in the control  
  char DisplayName[32] ;
  long lDefButBGColor ;
  long lDefButTextColor ;
  long lVersion ;
} ghk_t ;                  //  general house keeping  170 bytes    

ghk_t  ghk ;    // eprom backed up stuff 

typedef struct __attribute__((__packed__)) {             // permanent record
  char Display[MAX_DISPLAY] ;    //  say 8 ---> 7 + the /0
  uint32_t lCode ;               //  IR code to send  
  uint32_t lRxCode ;             //  code to trancieve
  uint32_t lBGColor ;            //  background color of buttons  
  uint16_t wRxTxType ;           //  type of RX (Hi byte) and TX (Lo byte) transmissions   
} IRPad_Button_t ;                      //  general house keeping  22 bytes    (44x = 1056 )   x2 =>  2112  + 170 => 2282

typedef struct __attribute__((__packed__)) {             // array
  IRPad_Button_t pb[MAX_PAD] ;
  char  Display[MAX_DISPLAY] ;    //  say 8 ---> 7 + the /0
  uint32_t lBGColor ;            //  background color of pad select buttons  
  uint32_t lBGHColor ;           //  background color of selected pad select buttons  
} IRPad_t ;

IRPad_t  IRPad[MAX_VR];               // three virtual remote pads that you can swap between

byte rtc_sec = 0 ;
byte rtc_min = 0 ;
byte rtc_hour = 0  ;
long lScanCtr = 0 ;
long lScanLast = 0 ;
bool  bNWG=false ;
long PrevCode ;
uint32_t webcode = 0 ;
uint16_t webtype = 0 ;
uint32_t ircode = 0 ;
uint16_t irtype = 0 ;
int iMode ;
long lTxNow ;
long irset = 0 ;
long lRebootCode = 0 ;
byte padselect = 0 ; 


String DecodeResults ;

void LoadParamsFromEEPROM(bool bLoad){
long eeAddress ;  
  if ( bLoad ) {
    eeAddress = 0 ;
    EEPROM.get(eeAddress,ghk);
    eeAddress += sizeof(ghk) ;
    EEPROM.get(eeAddress,IRPad);
    eeAddress += sizeof(IRPad) ;
    Serial.println("Final EEPROM Save adress " +String(eeAddress));       
  }else{
    eeAddress = 0 ;
    EEPROM.put(eeAddress,ghk);
    eeAddress += sizeof(ghk) ;
    EEPROM.put(eeAddress,IRPad);
    eeAddress += sizeof(IRPad) ;
    EEPROM.commit();  // save changes in one go ???
    Serial.println("Final EEPROM Save adress " +String(eeAddress));       
  }
}





void setup() {
  int j ;
  pinMode(D5,INPUT);         //  Pin for IR LED's
  pinMode(D2,OUTPUT);        //  Pin for IR LED's
  digitalWrite(D2,LOW);      //  Turn off LED's as no resister .. they get hot !!!    

  Serial.begin(115200);      // (19200,SERIAL_8E1) - data size = 8 bits , parity = Even , stop bit =  1bit
  pinMode(LED,OUTPUT);       //  D4 builtin LED

  pinMode(LEDR,OUTPUT);      //  D4 builtin LED
  pinMode(LEDG,OUTPUT);      //  D4 builtin LED
  pinMode(LEDB,OUTPUT);      //  D4 builtin LED
  digitalWrite(LEDR,HIGH);
  digitalWrite(LEDG,HIGH);
  digitalWrite(LEDB,HIGH);

  EEPROM.begin(sizeof(ghk)+sizeof(IRPad)+64);  // as long as under 4096 we are good !!!
  LoadParamsFromEEPROM(true);

  if ( MYVER != ghk.lVersion ) {
    BackInTheBoxMemory();         // load defaults if blank memory detected but dont save user can still restore from eeprom
    Serial.println("Loading memory defaults...");
  }
  delay(2000);
  WiFi.disconnect();
  Serial.println("Configuring soft access point...");
//  WiFi.mode(WIFI_AP_STA);  // we are having our cake and eating it eee har
  if ( ghk.cssid[0] == 0 || ghk.cssid[1] == 0 ){   // pick a default setup ssid if none
    sprintf(ghk.cssid,"Configure_%08X\0",ESP.getChipId());
    sprintf(ghk.cpassword,"\0");
  }
  WiFi.softAPConfig(ghk.MyIPC,ghk.MyIPC,IPAddress (255, 255, 255 , 0));  
  Serial.println("Starting access point...");
  Serial.print("SSID: ");
  Serial.println(ghk.cssid);
  Serial.print("Password: >");
  Serial.print(ghk.cpassword);
  Serial.println("<");
  if ( ghk.cpassword[0] == 0 ){
    WiFi.softAP((char*)ghk.cssid);                   // no passowrd
  }else{
    WiFi.softAP((char*)ghk.cssid,(char*) ghk.cpassword);
  }

  ghk.MyIPC = WiFi.softAPIP();  // get back the address to verify what happened
  Serial.print("Soft AP IP address: ");
  snprintf(buff, BUFF_MAX, ">> IP %03u.%03u.%03u.%03u <<", ghk.MyIPC[0],ghk.MyIPC[1],ghk.MyIPC[2],ghk.MyIPC[3]);      
  Serial.println(buff);
  
  Serial.println("client connecting to access point...");
  Serial.print("SSID: ");
  Serial.println(ghk.nssid);
  Serial.print("Password: >");
  Serial.print(ghk.npassword);
  Serial.println("<");
  if ( ghk.npassword[0] == 0 ){
    WiFi.begin((char*)ghk.nssid);                         // connect to unencrypted access point      
  }else{
    WiFi.begin((char*)ghk.nssid, (char*)ghk.npassword);   // connect to access point with encryption
  }
  while (( WiFi.status() != WL_CONNECTED ) && ( j < MAX_WIFI_TRY )) {
   j = j + 1 ;
    digitalWrite(LEDB,LOW);
    delay(5);
    digitalWrite(LEDB,HIGH);
   delay(495);
   Serial.print(".");
   digitalWrite(BUILTIN_LED,!digitalRead(BUILTIN_LED));
  } 

  if ( j >= MAX_WIFI_TRY ) {
   Serial.println("");
   Serial.println("Connection to " + String() + " Failed");
   WiFi.disconnect();
  }else{
    ghk.MyIP = WiFi.localIP() ;
    Serial.println("");
    Serial.print("Connected to " + String(ghk.nssid) + " IP ");
    snprintf(buff, BUFF_MAX, ">> IP %03u.%03u.%03u.%03u <<", ghk.MyIP[0],ghk.MyIP[1],ghk.MyIP[2],ghk.MyIP[3]);      
    Serial.println(buff) ;
    bNWG = true ;
  }

  if ( ghk.NodeName == 0 ){
    sprintf(ghk.NodeName,"Control_%08X\0",ESP.getChipId());  
  }
  server.on("/", handleRoot);
  server.on("/setup", handleSetup );
  server.on("/csetup", handleCodeSetup );
  server.on("/info", handleInfo );
  server.on("/EEPROM",DisplayEEPROM);
  server.onNotFound(handleNotFound);   
  server.begin();
  
  Serial.println("HTTP server started...");
  
  rtc_min = minute();
  rtc_sec = second();
  OTAWebUpdater.setup(&OTAWebServer);
  OTAWebServer.begin();  

  irsend.begin();  // setup send and drive the pin low to make sure leds are off
  irrecv.enableIRIn();  // Start the receiver
  randomSeed(now());
  lRebootCode = random(0,+2147483640) ;

}

void loop() {
long lRet ;
char c ;
bool  bTransmit ;
bool bRepeat ;

  server.handleClient();
  OTAWebServer.handleClient();
  lScanCtr++ ;

  if (( millis() > lTxNow )&& ( ircode != 0 )){
    webcode = ircode ;
    webtype = irtype ; 
    ircode = 0 ; 
  }
  
  if ( webcode != 0 ){
    irrecv.disableIRIn();  // Start the receiver
//    irsend.sendNEC(webcode, 32);
    irsend.send( webtype , webcode , 32);
    digitalWrite(LEDR,LOW);
    delay(1);
    digitalWrite(LEDR,HIGH);
    Serial.println("IR Send " + String(webcode,HEX));
    webcode = 0 ;
    webtype = NEC ;
    irrecv.enableIRIn();  // Start the receiver
  }
  
  if (second() != rtc_sec) {                                // do only once a second
    digitalWrite(BUILTIN_LED,!digitalRead(BUILTIN_LED));
//    if (( rtc_sec % 2 ) == 0 ){
//    }
    rtc_sec = second();
    lScanLast = lScanCtr ;
    lScanCtr = 0 ;
  }


  if (irrecv.decode(&results)) {
    dump(&results);
    digitalWrite(LEDG,LOW);
    delay(1);
    digitalWrite(LEDG,HIGH);
    for ( int  j = 0 ; j < MAX_VR ; j++ ){
      for ( int  i = 0 ; i < MAX_PAD ; i++ ){
        if (( IRPad[j].pb[i].lRxCode == (results.value & 0xFFFFFFFF))&&( IRPad[j].pb[i].lRxCode != 0 )){        
          ircode = IRPad[j].pb[i].lCode ;
          irtype = IRPad[j].pb[i].wRxTxType & 0xff ;
          lTxNow = millis() + ghk.lTxDelay ;
        }
      }
    }
    irrecv.resume();  // Receive the next value
  }

}   // bottom of loop



void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  uint16_t count = results->rawlen;
  DecodeResults = "" ;
  if (results->decode_type == UNKNOWN) {
    DecodeResults += "Unknown encoding: " ;
  } else if (results->decode_type == NEC) {
    DecodeResults += "Decoded NEC: ";
  } else if (results->decode_type == SONY) {
    DecodeResults += "Decoded SONY: ";
  } else if (results->decode_type == RC5) {
    DecodeResults += "Decoded RC5: ";
  } else if (results->decode_type == RC5X) {
    DecodeResults += "Decoded RC5X: ";
  } else if (results->decode_type == RC6) {
    DecodeResults += "Decoded RC6: ";
  } else if (results->decode_type == RCMM) {
    DecodeResults += "Decoded RCMM: ";
  } else if (results->decode_type == PANASONIC) {
    DecodeResults += "Decoded PANASONIC - Address: ";
    DecodeResults += String(results->address, HEX);
    DecodeResults += " Value: ";
  } else if (results->decode_type == LG) {
    DecodeResults += "Decoded LG: ";
  } else if (results->decode_type == JVC) {
    DecodeResults += "Decoded JVC: ";
  } else if (results->decode_type == AIWA_RC_T501) {
    DecodeResults += "Decoded AIWA RC T501: ";
  } else if (results->decode_type == WHYNTER) {
    DecodeResults += "Decoded Whynter: ";
  } else if (results->decode_type == NIKAI) {
    DecodeResults += "Decoded Nikai: ";
  }
  DecodeResults += String((uint32_t)results->value, HEX) ;  
  DecodeResults += " (" ;
  DecodeResults += String((uint32_t)results->bits, DEC) ;
  DecodeResults += " bits)" ;
  DecodeResults += "Raw (" ;
  DecodeResults += String(count, DEC) ;
  DecodeResults += "): {" ;

  for (uint16_t i = 1; i < count; i++) {
    if (i % 100 == 0)
      yield();  // Preemptive yield every 100th entry to feed the WDT.
    if (i & 1) {
      DecodeResults += String(results->rawbuf[i] * kRawTick, DEC);
    } else {
      DecodeResults += ", " ;
      DecodeResults += String((uint32_t) results->rawbuf[i] * kRawTick, DEC);
    }
  }
  DecodeResults += "};" ;
}

void DecodeTypeLookup(long decode_type,char *buff){
 
  switch(decode_type){
    case NEC:
      sprintf(buff,"NEC\0") ;
    break;    
    case SONY:
      sprintf(buff,"SONY\0") ;
    break;  
    case RC5:
      sprintf(buff,"RC5\0") ;
    break;  
    case RC5X:
      sprintf(buff,"RC5X\0") ;
    break;  
    case RC6:
      sprintf(buff,"RC6\0") ;
    break;  
    case RCMM:
      sprintf(buff,"RCMM\0") ;
    break;  
    case PANASONIC:
      sprintf(buff,"PANASONIC\0") ;
    break;  
    case LG:
      sprintf(buff,"LG\0") ;
    break;  
    case JVC:
      sprintf(buff,"JVC\0") ;
    break;  
    case AIWA_RC_T501:
      sprintf(buff,"AIWA RC T501\0") ;
    break;  
    case WHYNTER:
      sprintf(buff,"WHYNTER\0") ;
    break;  
    case NIKAI:
      sprintf(buff,"NIKAI\0") ;
    break;  
    default:
      sprintf(buff,"-\0") ;
    break;
  }
  
}

