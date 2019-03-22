void SendHTTPHeader(bool bIncludeStyle){ 
  char buff[12];
  
  server.sendHeader("Server","ESP8266-on-steroids",false);
  server.sendHeader("X-Powered-by","Dougal-1.0",false);
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");
  server.sendContent("<!DOCTYPE HTML>");
  server.sendContent("<head><title>Team Trouble - IR Transceiver</title>");
  server.sendContent("<meta name=viewport content='width=320, auto inital-scale=1'>");
  if ( bIncludeStyle ) {
    sprintf(buff,"%06X",ghk.lDefButBGColor);
    server.sendContent("<style>input[type=submit] { border-radius:8px; color:#"+String(ghk.lDefButTextColor,HEX)+";background-color:#"+String(buff)+"; height:"+String(ghk.butH)+"px; width:"+String(ghk.butW)+"px ; } </style>");
//    server.sendContent("<style>input[type=submit] { border-radius:8px; color:#"+String(ghk.lDefButTextColor,HEX)+"; height:"+String(ghk.butH)+"px; width:"+String(ghk.butW)+"px ; } </style>");
  }
  server.sendContent("</head><body><html><center>");
}

void SendHTTPPageFooter(){
  server.sendContent("<h2>"+String(ghk.DisplayName)+"</h2><a href='/'>Refresh</a><br><br>") ;         
  server.sendContent(F("<br><a href='/?command=2'>Save Parameters to EEPROM</a><br>")) ;         
  server.sendContent(F("<a href='/?command=1'>Load Parameters from EEPROM</a><br>"));
  server.sendContent(F("<a href='/?command=667'>Reset Memory to Factory Default</a><br>"));
  server.sendContent(F("<a href='/?command=9'>Reboot</a><br>"));   
  server.sendContent(F("<a href='/setup'>General Setup</a><br>"));  
  server.sendContent(F("<a href='/info'>Info</a><br>"));  
  server.sendContent(F("<a href='/csetup'>Code Setup</a><br>"));  
  snprintf(buff, BUFF_MAX, "%u.%u.%u.%u", ghk.MyIP[0],ghk.MyIP[1],ghk.MyIP[2],ghk.MyIP[3]);
  server.sendContent("<br><a href='http://" + String(buff) + ":81/update'>OTA Firmware Update</a><br>");
  server.sendContent(F("<br><a href='/EEPROM'>EEPROM Contents</a>"));  
  server.sendContent(F("</body></html>\r\n"));
}

void SendFormTag(){
  server.sendContent("<form method=get action=" + server.uri() + ">");
}

void handleNotFound(){
  String message = "Seriously - No way DUDE\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.print(message);
}

 
void handleRoot() {
  boolean currentLineIsBlank = true;
  char buff[40] ;
  String MyColor ;
  String MyText ;
  long  i = 0 ;
  int k = 0 ;
  bool bDefault = true ;
  String message ;

//  SerialOutParams();
  
  for (uint8_t j=0; j<server.args(); j++){
    i = String(server.argName(j)).indexOf("command");
    if (i != -1){  // have a request to set the time zone
      switch (String(server.arg(j)).toInt()){
        case 1:  // load values
          LoadParamsFromEEPROM(true);
          Serial.println("Load from EEPROM");
        break;
        case 2: // Save values
          LoadParamsFromEEPROM(false);
          Serial.println("Save to EEPROM");
        break;
        case 8: //  Cold Reboot
          ESP.reset() ;
        break;
        case 9: //  Warm Reboot
          ESP.restart() ;
        break;       

        case 42:
          ESP.reset() ;
        break;
      }  
    }
    if (server.argName(j) == "type") {
      webtype = strtoul(server.arg(j).c_str(), NULL, 10);
    }
    
    if (server.argName(j) == "code") {
      webtype = NEC ;
      webcode = strtoul(server.arg(j).c_str(), NULL, 10);
      digitalWrite(LEDB,LOW);
      delay(1);
      digitalWrite(LEDB,HIGH);
//      Serial.println("IR Send RX " + String(webcode,HEX));
    }
   
  }

  SendHTTPHeader(true);   //  ################### START OF THE RESPONSE  ######
  if (bDefault) {         //  #####################################   Default Control ##############################################    
    server.sendContent("<table border=0 title='IR Tranmitter'>"); 

    for ( k = 0 ; k < MAX_PAD ; k++ ){
      if (( k % MAX_COLS ) == 0 ){
        server.sendContent("<tr>") ;        
      }
      if ( IRPad[k].lCode == 0 ){
        server.sendContent( "<td align=center title='"+String(k)+"'></td>" ) ;        
      }else{
//        server.sendContent( "<td align=center title='"+String(k)+"'><form method=get action=" + server.uri() + "><input type='submit' value='"+String(IRPad[k].Display)+"' style='border-radius:8px; color:#"+String(ghk.lDefButTextColor,HEX)+";background-color:#"+String(ghk.lDefButBGColor,HEX)+"; height:"+String(ghk.butH)+"px; width:"+String(ghk.butW)+"px'><input type='hidden' name='code' value='"+String(IRPad[k].lCode)+"'></form></td>" ) ;
        if (IRPad[k].lBGColor != 0 ) {
          sprintf(buff,"%06X",IRPad[k].lBGColor);
          server.sendContent( "<td align=center title='"+String(k)+"'><form method=get action=" + server.uri() + "><input type='submit' value='"+String(IRPad[k].Display)+"' style='background-color:#"+String(buff)+"'><input type='hidden' name='code' value='"+String(IRPad[k].lCode)+"'><input type='hidden' name='type' value='"+String(IRPad[k].wRxTxType & 0xff)+"'></form></td>" ) ;          
        }else{
//          server.sendContent( "<td align=center title='"+String(k)+"'><form method=get action=" + server.uri() + "><input type='submit' value='"+String(IRPad[k].Display)+"' style='background-color:#"+String(ghk.lDefButBGColor,HEX)+"'><input type='hidden' name='code' value='"+String(IRPad[k].lCode)+"'></form></td>" ) ;                    
          server.sendContent( "<td align=center title='"+String(k)+"'><form method=get action=" + server.uri() + "><input type='submit' value='"+String(IRPad[k].Display)+"' ><input type='hidden' name='code' value='"+String(IRPad[k].lCode)+"'><input type='hidden' name='type' value='"+String(IRPad[k].wRxTxType & 0xff)+"'></form></td>" ) ;                    
        }
      }
      if (( k % MAX_COLS ) == ( MAX_COLS - 1 )){
        server.sendContent( "</tr>" ) ;
      }
    
    }
    server.sendContent("</table>"); 
  }
  SendHTTPPageFooter();
}


void SerialOutParams(){
String message ;
   
  message = "Web Request URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  Serial.println(message);
  message = "";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  Serial.println(message);
}





void handleRoot2() {
  server.send(200, "text/html",
              "<html>" \
                "<head><title>ESP8266 Demo</title></head>" \
                "<body>" \
                  "<h1>Hello from ESP8266, you can send NEC encoded IR" \
                      "signals from here!</h1>" \
                  "<p><a href=\"ir?code=16769055\">Send 0xFFE01F</a></p>" \
                  "<p><a href=\"ir?code=16429347\">Send 0xFAB123</a></p>" \
                  "<p><a href=\"ir?code=16771222\">Send 0xFFE896</a></p>" \
                "</body>" \
              "</html>");
}

void handleIr() {
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "code") {
      uint32_t code = strtoul(server.arg(i).c_str(), NULL, 10);
#if SEND_NEC
      irsend.sendNEC(code, 32);
#endif  // SEND_NEC
    }
  }
  handleRoot();
}


