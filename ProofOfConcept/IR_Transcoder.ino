//------------------------------------------------------------------------------
// Include the IRremote library header
//
//#include <IRremoteESP8266.h>
//#include <IRsend.h>

#include <IRremote.h>

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

  
IRsend irsend;
long PrevCode;
int iMode = 0;
bool bTransmit=false;
bool bRepeat=false;
//------------------------------------------------------------------------------
// Tell IRremote which Arduino pin is connected to the IR Receiver (TSOP4838)
//
int recvPin = 11;
IRrecv irrecv(recvPin);


//+=============================================================================
// Configure the Arduino
//
void  setup ( )
{
  Serial.begin(115200);   // Status message will be sent to PC at 9600 baud
  Serial.println("Start");
  pinMode(3,OUTPUT);  // IR LED
  pinMode(4,OUTPUT);  // RGB LED FOR MODE INDICATOR
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  irrecv.enableIRIn();  // Start the receiver
  irrecv.blink13(1);
}

//+=============================================================================
// Display IR code
//
void  ircode (decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    Serial.print(results->address, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:      Serial.print("UNKNOWN");       break ;
    case NEC:          Serial.print("NEC");           break ;
    case SONY:         Serial.print("SONY");          break ;
    case RC5:          Serial.print("RC5");           break ;
    case RC6:          Serial.print("RC6");           break ;
    case DISH:         Serial.print("DISH");          break ;
    case SHARP:        Serial.print("SHARP");         break ;
    case JVC:          Serial.print("JVC");           break ;
    case SANYO:        Serial.print("SANYO");         break ;
    case MITSUBISHI:   Serial.print("MITSUBISHI");    break ;
    case SAMSUNG:      Serial.print("SAMSUNG");       break ;
    case LG:           Serial.print("LG");            break ;
    case WHYNTER:      Serial.print("WHYNTER");       break ;
    case AIWA_RC_T501: Serial.print("AIWA_RC_T501");  break ;
    case PANASONIC:    Serial.print("PANASONIC");     break ;
    case DENON:        Serial.print("Denon");         break ;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWLEN");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpRaw (decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen-1, DEC);
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen-1) Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  Serial.println("");
  }
  Serial.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");          // variable type
  Serial.print("rawData[");                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen-1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }

  // End declaration
  Serial.print("};");  // 

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}

//+=============================================================================
// The repeating section of the code
//
void  loop ( )
{
  decode_results  results;        // Somewhere to store the results

  if (irrecv.decode(&results)) {  // Grab an IR code
    dumpInfo(&results);           // Output the results
    dumpRaw(&results);            // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("");           // Blank line between entries

    
//    Serial.println(iMode);
    Serial.print("R ");
    Serial.println(results.value, HEX);

    bTransmit = false ;
    switch(results.value){
      case 0x18E710EF:   // TV on off
          PrevCode = TVONOFF ;
          bTransmit = true ;
          bRepeat = false ;
          iMode = 0 ;
      break;
      case 0x18E750AF:    // AV
          PrevCode = TVAV ;
          bTransmit = true ;
          bRepeat = false ;
          iMode = 0 ;
      break;
      case 0x18E740BF:   // VOL +
          PrevCode = TVVOLUP ;
          bTransmit = true ;
          bRepeat = true ;
          iMode = 0 ;
      break;
      case 0x18E7C03F:  // VOL -
          PrevCode = TVVOLDN ;
          bTransmit = true ;
          bRepeat = true ;
          iMode = 0 ;
      break;
      case 0x18E700FF:   // CH +
          PrevCode = TVCHUP ;
          bTransmit = true ;
          bRepeat = false ;
          iMode = 0 ;
      break;
      case 0x18E7807F:    // CH-
          PrevCode = TVCHDN ;
          bTransmit = true ;
          bRepeat = false ;
          iMode = 0 ;
      break;
      case 0xFFFFFFFF:    // repeat code
          if ( bRepeat == true ){
            bTransmit = true ;
          }
      break;

      case 0xD880287:  // blue button
        if ( iMode != 1 ){
          iMode = 1;
        }else{
          iMode = 0;
        }
        bRepeat = false ;
      break;
      case 0xD88C247:  // yellow button
          PrevCode = TVMUTE ;
          bTransmit = true ;
          bRepeat = false ;
          iMode = 0 ;
      break;
      case 0xD8842C7:  // green button
        if ( iMode != 3 ){
          iMode = 2;
        }else{
          iMode = 0;
        }
        bRepeat = false ;        
      break;
      case 0xD888207:  // red button
        if ( iMode != 4 ){
          iMode = 3;
        }else{
          iMode = 0;
        }
        bRepeat = false ;        
      break;
      case 0xD08E1E4:  // left nav
        if ( iMode == 1 ){
          PrevCode = TVLNAV ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD081114:  // right nav
        if ( iMode == 1 ){
          PrevCode = TVRNAV ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD086164:  // down nav
        if ( iMode == 1 ){
          PrevCode = TVDNAV ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08A1A4:  // up nav
        if ( iMode == 1 ){
          PrevCode = TVUNAV ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD084144:  // ok nav
        if ( iMode == 1 ){
          PrevCode = TVOKNAV ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD888A0F:  // exit nav
        if ( iMode == 1 ){
          PrevCode = TVEXITNAV ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD086663:  // status nav
          bRepeat = false ;
      break;
      case 0xD088184:  // return nav
          bRepeat = false ;
      break;
      case 0xD08080D:  // 1 nav pad
        if ( iMode == 1 ){
          PrevCode = TV1PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08888D:  // 2 nav pad
        if ( iMode == 1 ){
          PrevCode = TV2PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08484D:  // 3 nav pad
        if ( iMode == 1 ){
          PrevCode = TV3PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08C8CD:  // 4 nav pad
        if ( iMode == 1 ){
          PrevCode = TV4PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08282D:  // 5 nav pad
        if ( iMode == 1 ){
          PrevCode = TV5PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08A8AD:  // 6 nav pad
        if ( iMode == 1 ){
          PrevCode = TV6PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08686D:  // 7 nav pad
        if ( iMode == 1 ){
          PrevCode = TV7PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08E8ED:  // 8 nav pad
        if ( iMode == 1 ){
          PrevCode = TV8PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08181D:  // 9 nav pad
        if ( iMode == 1 ){
          PrevCode = TV9PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      case 0xD08989D:  // 0 nav pad
        if ( iMode == 1 ){
          PrevCode = TV0PAD ;
          bTransmit = true ;
          bRepeat = false ;
        }
      break;
      default:
          bTransmit = false ;
          bRepeat = false ;
      break;
    }
    if ( bTransmit ){
      Serial.print("T ");
      Serial.println(PrevCode, HEX);
      irsend.sendNEC(PrevCode,32);    
      bTransmit = false ;
    }
    delay(50);
    irrecv.resume();              // Prepare for the next value
        
    irrecv.enableIRIn();            // Start the receiver
    irrecv.blink13(1);

  }

}
