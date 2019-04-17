void handleSetup(){
  int i , ii , iTmp , iX ;
  uint8_t j , k , kk ;
  String message ;  
  String MyNum ;  
  String MyColor ;
  String MyColor2 ;
  byte mac[6];

//  SerialOutParams();

  for (uint8_t j=0; j<server.args(); j++){
    i = String(server.argName(j)).indexOf("command");
    if (i != -1){  // 
      switch (String(server.arg(j)).toInt()){
        case 1:  // load values
          LoadParamsFromEEPROM(true);
        break;
        case 2: // Save values
          LoadParamsFromEEPROM(false);
        break;
        case 3: // 
        break;
        case 4: // 
        break;
        case 5: // 
        break;
        case 8: //  Cold Reboot
          ESP.reset() ;
        break;
        case 9: //  Warm Reboot
          ESP.restart() ;
        break;
        case 667: // wipe the memory to factory default
          BackInTheBoxMemory();
        break;
      }  
    }
   
    
    i = String(server.argName(j)).indexOf("bscip");
    if (i != -1){  // have a request to request an IP address
      ghk.MyIP[0] = String(server.arg(j)).substring(0,3).toInt() ;
      ghk.MyIP[1] =String(server.arg(j)).substring(4,7).toInt() ;
      ghk.MyIP[2] = String(server.arg(j)).substring(8,11).toInt() ;
      ghk.MyIP[3] =String(server.arg(j)).substring(12,15).toInt() ;
    }
    i = String(server.argName(j)).indexOf("bsaip");
    if (i != -1){  // have a request to request an IP address
      ghk.MyIPC[0] = String(server.arg(j)).substring(0,3).toInt() ;
      ghk.MyIPC[1] =String(server.arg(j)).substring(4,7).toInt() ;
      ghk.MyIPC[2] = String(server.arg(j)).substring(8,11).toInt() ;
      ghk.MyIPC[3] =String(server.arg(j)).substring(12,15).toInt() ;
    }
    
    i = String(server.argName(j)).indexOf("nssid");
    if (i != -1){                                    // SSID
     String(server.arg(j)).toCharArray( ghk.nssid , sizeof(ghk.nssid)) ;
    }
    i = String(server.argName(j)).indexOf("npass");
    if (i != -1){                                    // Password
     String(server.arg(j)).toCharArray( ghk.npassword , sizeof(ghk.npassword)) ;
    }
    
    i = String(server.argName(j)).indexOf("cssid");
    if (i != -1){                                    // SSID
     String(server.arg(j)).toCharArray( ghk.cssid , sizeof(ghk.cssid)) ;
    }
    i = String(server.argName(j)).indexOf("cpass");
    if (i != -1){                                    // Password
     String(server.arg(j)).toCharArray( ghk.cpassword , sizeof(ghk.cpassword)) ;
    }
    
    i = String(server.argName(j)).indexOf("dname");
    if (i != -1){                                    // Display Name at top of page
     String(server.arg(j)).toCharArray( ghk.DisplayName , sizeof(ghk.DisplayName)) ;
    }

    i = String(server.argName(j)).indexOf("bgcol");
    if (i != -1){                                    // background color
     ghk.lDefButBGColor = String(server.arg(j)).toInt() ;
    }
    i = String(server.argName(j)).indexOf("txcol");
    if (i != -1){                                    // background color
     ghk.lDefButTextColor = String(server.arg(j)).toInt() ;
    }

    i = String(server.argName(j)).indexOf("rx2tx");
    if (i != -1){                                    // RX 2 TX delay (ms)
     ghk.lTxDelay = String(server.arg(j)).toInt() ;
     constrain(ghk.lTxDelay,5,1000);
    }
    
    i = String(server.argName(j)).indexOf("butwd");
    if (i != -1){                                    // Button Width (pixels)
     ghk.butW = String(server.arg(j)).toInt() ;
     constrain(ghk.butW,20,400);
    }
    i = String(server.argName(j)).indexOf("butht");
    if (i != -1){                                    // Button Height (pixels)
     ghk.butH = String(server.arg(j)).toInt() ;
     constrain(ghk.butH,20,400);
    }
    i = String(server.argName(j)).indexOf("butco");
    if (i != -1){                                    // Button Columns (pixels)
     ghk.butC = String(server.arg(j)).toInt() ;
     constrain(ghk.butC,1,8);
    }
    
    

  
  }  // for j searching through parameters

  SendHTTPHeader(false);

  server.sendContent(F("<br><center><b>General Setup Node</b><br>"));
  server.sendContent(F("<b>Setup</b><br><table border=1 title='General Setup'>"));
  server.sendContent(F("<tr><th>Parameter</th><th>Value</th><th colspan=2>Units</th></tr>"));

  server.sendContent(F("<tr><td>Rx to Tx Delay</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='rx2tx' value='" + String(ghk.lTxDelay) + "' size=12></td><td>(ms)</td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Button Width</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='butwd' value='" + String(ghk.butW) + "' size=12></td><td>(Pixels)</td><td><input type='submit' value='SET'></form></td></tr>");
  server.sendContent(F("<tr><td>Button Height</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='butht' value='" + String(ghk.butH) + "' size=12></td><td>(Pixels)</td><td><input type='submit' value='SET'></form></td></tr>");
  server.sendContent(F("<tr><td>Number of Button Columns</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='butco' value='" + String(ghk.butC) + "' size=12></td><td>.</td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Background Color</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='bgcol' value='" + String(ghk.lDefButBGColor,HEX) + "' size=12></td><td>(Hex)</td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Text Color</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='txcol' value='" + String(ghk.lDefButTextColor,HEX) + "' size=12></td><td>(Hex)</td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Display Name</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='dname' value='" + String(ghk.DisplayName) + "' maxlength=15 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>SSID</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='nssid' value='" + String(ghk.nssid) + "' maxlength=15 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Password</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='npass' value='" + String(ghk.npassword) + "' maxlength=15 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>SSID</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='cssid' value='" + String(ghk.cssid) + "' maxlength=15 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>");

  server.sendContent(F("<tr><td>Password</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='cpass' value='" + String(ghk.cpassword) + "' maxlength=15 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>");

  snprintf(buff, BUFF_MAX, "%03u.%03u.%03u.%03u", ghk.MyIP[0],ghk.MyIP[1],ghk.MyIP[2],ghk.MyIP[3]);
  server.sendContent( F("<tr><td>Remote IP Address Control</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent( "<input type='text' name='bscip' value='" + String(buff) + "' maxlength=16 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>") ;

  snprintf(buff, BUFF_MAX, "%03u.%03u.%03u.%03u", ghk.MyIPC[0],ghk.MyIPC[1],ghk.MyIPC[2],ghk.MyIPC[3]);
  server.sendContent(F("<tr><td>Remote IP Address Control</td><td align=center>")) ; 
  SendFormTag();
  server.sendContent("<input type='text' name='bsaip' value='" + String(buff) + "' maxlength=16 size=12></td><td></td><td><input type='submit' value='SET'></form></td></tr>") ;

  server.sendContent(F("</table><br>"));    
  SendHTTPPageFooter();
}



