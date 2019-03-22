void handleCodeSetup(){
  int i , ii , iTmp , iX ;
  uint8_t j , k , kk ;
  String message ;  
  String MyNum ;  
  String MyColor ;
  String MyColor2 ;

  SerialOutParams();

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
    if (i != -1){                                    // button number
     k = String(server.arg(j)).toInt() ;
    }
    
    
    i = String(server.argName(j)).indexOf("descr");
    if (i != -1){                                    // Display Name at top of page
     String(server.arg(j)).toCharArray( IRPad[k].Display , sizeof(IRPad[k].Display)) ;
    }

    i = String(server.argName(j)).indexOf("txcod");
    if (i != -1){                                    // transmitt code
     IRPad[k].lCode = String(server.arg(j)).toInt() ;
    }
    
    i = String(server.argName(j)).indexOf("rxcod");
    if (i != -1){                                    // recieve code
     IRPad[k].lRxCode = String(server.arg(j)).toInt() ;
    }
    
    i = String(server.argName(j)).indexOf("bgcol");
    if (i != -1){                                    // background color
     ii = String(server.arg(j)).indexOf("0x");
     if ( ii != -1 ) {
       sscanf((char *)String(server.arg(j)).c_str(),"%X",&IRPad[k].lBGColor);
     }else{
       IRPad[k].lBGColor = String(server.arg(j)).toInt() ;
     }
    }
    

    i = String(server.argName(j)).indexOf("irset");
    if (i != -1){                                    // button to set
     irset = String(server.arg(j)).toInt() ;
    }
    i = String(server.argName(j)).indexOf("rxset");
    if (i != -1){                                    // set last code to rx on button
     ii = results.decode_type ; 
     IRPad[irset].wRxTxType &=  0x00ff  ;
     IRPad[irset].wRxTxType |= (( ii & 0xff ) << 8 ) ;
     IRPad[irset].lRxCode = (uint32_t)results.value ;
    }
    i = String(server.argName(j)).indexOf("txset");
    if (i != -1){                                    // set last code to tx on button
     ii = results.decode_type ; 
     IRPad[irset].wRxTxType &=  0xff00  ;
     IRPad[irset].wRxTxType |= ( ii & 0xff ) ;
     IRPad[irset].lCode = (uint32_t)results.value ;
    }
    i = String(server.argName(j)).indexOf("wtxxx");
    if (i != -1){                                    // set last code to tx on button
     ii = String(server.arg(j)).toInt() ; 
     IRPad[irset].wRxTxType &= 0xff00  ;
     IRPad[irset].wRxTxType |= ( ii & 0xff ) ;
//     Serial.print("SSID: ");     
    }
    i = String(server.argName(j)).indexOf("wrxxx");
    if (i != -1){                                    // set last code to tx on button
     ii = String(server.arg(j)).toInt() ; 
     IRPad[irset].wRxTxType &= 0xff   ;
     IRPad[irset].wRxTxType |= ((ii & 0xff ) << 8 ) ;
    }    
  
  }  // for j searching through parameters

  SendHTTPHeader(false);

  server.sendContent(F("<h2><center><b>Virtual Remote - Code Setup</b></h2><br>"));
  server.sendContent(F("<table border=1 title='Code Setup'>"));
  server.sendContent(F("<tr><th align=center colspan=3>Button</th><th colspan=2>IR Codes<th colspan=4>IR Type</th><th rowspan=2>.</th></tr>"));
  server.sendContent(F("<tr><th>Number</th><th>Desciption</th><th>Color</th><th>Rx</th><th>Tx</th><th colspan=2>Rx</th><th colspan=2>Tx</th></tr>"));
  for ( i = 0 ; i < MAX_PAD ; i++ ){
    server.sendContent("<tr>");
    SendFormTag();
    if ( IRPad[i].lCode == 0 ){
      MyColor = "" ;
      MyColor2 = "" ;
    }else{
      if (IRPad[i].lBGColor != 0 ) {
        sprintf(buff,"%06X",IRPad[i].lBGColor);
      }else{
        sprintf(buff,"%06X",ghk.lDefButBGColor);      
      }
      MyColor = String("bgcolor=")+String(buff) ;
      if (IRPad[i].lBGColor != 0 ) {                          // xor the color to get contrasting color
        sprintf(buff,"%06X",IRPad[i].lBGColor ^ 0xffffff );
      }else{
        sprintf(buff,"%06X",ghk.lDefButBGColor ^ 0xffffff );      
      }
      MyColor2 = String("color=")+String(buff) ;
    }
    server.sendContent("<td "+MyColor+" align=center><a "+MyColor2+" href='"+server.uri()+"?irset="+String(i)+"'>"+String(i)+"</a><input type='hidden' name='irnum' value='"+String(i)+"'></td>");
    server.sendContent("<td><input type='text' name='descr' value='"+String(IRPad[i].Display)+"' maxlength=7 size=7></td>");
    server.sendContent("<td "+MyColor+"><input type='text' name='bgcol' value='"+String(IRPad[i].lBGColor)+"' size=6></td>");
    server.sendContent("<td><input type='text' name='rxcod' value='"+String(IRPad[i].lRxCode)+"' size=6></td>");  
    server.sendContent("<td><input type='text' name='txcod' value='"+String(IRPad[i].lCode)+"' size=6></td>") ;
    DecodeTypeLookup((long)((IRPad[i].wRxTxType & 0xff00 )>> 8 ) ,(char *)&buff[0]);
    server.sendContent("<td><input type='text' name='wrxxx' value='"+String(((IRPad[i].wRxTxType & 0xff00 )>> 8 ))+"' maxlength=2 size=2></td><td align=center>"+String(buff)+"</td>");  
    DecodeTypeLookup((long)(IRPad[i].wRxTxType & 0xff ) ,(char *)&buff[0]);
    server.sendContent("<td><input type='text' name='wtxxx' value='"+String((IRPad[i].wRxTxType & 0xff ))+"' maxlength=2 size=2></td><td align=center>"+String(buff)+"</td><td><input type='submit' value='SET'></td></form></tr>");  
  }
  server.sendContent(F("</table><br>"));  
  
  server.sendContent(F("<b>Last IR Code Recieved</b>"));
  server.sendContent(F("<table border=1 title='Last IR Code Recieved'>"));
  DecodeTypeLookup((long)results.decode_type ,(char *)&buff[0]);
  server.sendContent("<tr><th rowspan=2>Parameter</th><th colspan=2>Value - "+String(buff)+"</th></tr>");  
  server.sendContent(F("<tr><th>Hex</th><th>Decimal</th></tr>"));  
  server.sendContent("<tr><td>Code Type</td><td>"+String((uint32_t)results.decode_type, HEX)+"</td><td>"+String((uint32_t)results.decode_type, DEC)+"</td></tr>");  
  server.sendContent("<tr><td>Code Value</td><td>"+String((uint32_t)results.value, HEX)+"</td><td>"+String((uint32_t)results.value, DEC)+"</td></tr>");  
  server.sendContent("<tr><td>Code Address</td><td>"+String((uint32_t)results.address, HEX)+"</td><td>"+String((uint32_t)results.address, DEC)+"</td></tr>");  
  server.sendContent("<tr><td>Code Bits</td><td>"+String((uint32_t)results.bits, HEX)+"</td><td>"+String((uint32_t)results.bits, DEC)+"</td></tr>");  
  server.sendContent(F("</table><br>"));  
  if ( String(DecodeResults).length() > 0 ){
    server.sendContent(F("<b>Transfer Last scan to Buttons</b>"));
    server.sendContent(F("<table title='Set last code to'>"));
    SendFormTag();
    server.sendContent("<tr><td><b>Button:</b></td><td><input type='text' name='irset' value='"+String(irset)+"' maxlength=2 size=3></td><td><input type='submit' value='SET'></td></form></tr>");  
    SendFormTag();
    server.sendContent("<tr><td colspan=3><input type='hidden' name='irset' value='"+String(irset)+"'><input type='hidden' name='rxset' value='1'><input type='submit' value='Send Last Code To RX Of Button '" + String(irset) + "></form></td></tr>");  
    SendFormTag();
    server.sendContent("<tr><td colspan=3><input type='hidden' name='irset' value='"+String(irset)+"'><input type='hidden' name='txset' value='1'><input type='submit' value='Send Last Code To TX Of Button '" + String(irset) + "></form></td></tr>");  
    server.sendContent(F("</table>"));
    server.sendContent(F("<br><b>Verbose Last Decode Results</b><br><br>"));  
    server.sendContent(DecodeResults);
  }
  server.sendContent(F("<br><br>"));      
  SendHTTPPageFooter();

}
