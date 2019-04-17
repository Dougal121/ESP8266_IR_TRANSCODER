void handleCodeSetup(){
  int i , ii , iTmp , iX ;
  uint8_t j , k , kk ;
  String message ;  
  String MyNum ;  
  String MyColor ;
  String MyColor2 ;

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
   
    i = String(server.argName(j)).indexOf("irnum");
    if (i != -1){                                    // background color
     k = String(server.arg(j)).toInt() ;
    }
    
    
    i = String(server.argName(j)).indexOf("descr");
    if (i != -1){                                    // Display Name at top of page
     String(server.arg(j)).toCharArray( IRPad[k].Display , sizeof(IRPad[k].Display)) ;
    }

    i = String(server.argName(j)).indexOf("txcod");
    if (i != -1){                                    // background color
     IRPad[k].lCode = String(server.arg(j)).toInt() ;
    }
    
    i = String(server.argName(j)).indexOf("rxcod");
    if (i != -1){                                    // background color
     IRPad[k].lRxCode = String(server.arg(j)).toInt() ;
    }
    
    i = String(server.argName(j)).indexOf("bgcol");
    if (i != -1){                                    // background color
     IRPad[k].lBGColor = String(server.arg(j)).toInt() ;
    }
    
    i = String(server.argName(j)).indexOf("wrxtx");
    if (i != -1){                                    // background color
     IRPad[k].wRxTxType = String(server.arg(j)).toInt() ;
    }
  
  }  // for j searching through parameters

  SendHTTPHeader(false);

  server.sendContent(F("<h2><center><b>Code Setup</b></h2><br>"));
  server.sendContent(F("<table border=1 title='Code Setup'>"));
  server.sendContent(F("<tr><th>Button</th><th>Desciption</th><th>Tx Code</th><th>Rx Code</th><th>Color</th><th>Rx-Tx</th></tr>"));
  for ( i = 0 ; i < MAX_PAD ; i++ ){
    server.sendContent("<tr>");
    SendFormTag();
    server.sendContent("<td align=center>"+String(i)+"<input type='hidden' name='irnum' value='"+String(i)+"'></td><td><input type='text' name='descr' value='"+String(IRPad[i].Display)+"' maxlength=7 size=7></td>");
    server.sendContent("<td><input type='text' name='txcod' value='"+String(IRPad[i].lCode)+"' size=6></td>") ;
    server.sendContent("<td><input type='text' name='rxcod' value='"+String(IRPad[i].lRxCode)+"' size=6></td>");  
    server.sendContent("<td><input type='text' name='bgcol' value='"+String(IRPad[i].lBGColor,HEX)+"' size=6></td>");
    server.sendContent("<td><input type='text' name='wrxtx' value='"+String(IRPad[i].wRxTxType,HEX)+"' size=6></td><td><input type='submit' value='SET'></td></form></tr>");  
  }
  server.sendContent(F("</table><br>"));  

  server.sendContent(F("<table border=1 title='Last IR Code Recieved'>"));
  server.sendContent(F("<tr><th>Parameter</th><th>Value</th></tr>"));  
  server.sendContent(F("<tr><td>Code Type</td><td></td></tr>"));  
  server.sendContent(F("<tr><td>Code Value</td><td></td></tr>"));  
  server.sendContent(F("<tr><td>Code Address</td><td></td></tr>"));  
  server.sendContent(F("<tr><td>Code Bits</td><td></td></tr>"));  
  server.sendContent(F("</table><br>Verbose Last Decode Results<br><br>"));  
  server.sendContent(DecodeResults);
  server.sendContent(F("<br><br>"));      
  SendHTTPPageFooter();

}
