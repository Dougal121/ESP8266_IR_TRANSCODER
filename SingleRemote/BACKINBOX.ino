void BackInTheBoxMemory(){
  sprintf(ghk.NodeName,"Control_%08X\0",ESP.getChipId());  
  sprintf(ghk.npassword,"\0" ) ;
  sprintf(ghk.cssid,"Control_%08X\0",ESP.getChipId());  
  sprintf(ghk.cpassword,"\0" ) ;
//  ghk.lDisplayOptions = 0 ; 
  ghk.lTxDelay = 50 ;
  ghk.butW = 75 ;
  ghk.butH = 50 ;
  ghk.butC = 4 ;
  ghk.lDefButBGColor = 0xB2FF66 ;      // olivy color WTF 
  ghk.lDefButTextColor = 0x000000 ;    // black
  ghk.MyIP = IPAddress(192,168,1,111) ;                                  // main
  ghk.MyIPC = IPAddress(192, 168, (5 +(ESP.getChipId() & 0x7f )) , 1) ;  // configuration              
  sprintf(ghk.npassword,"********\0");   // your default credentials here
  sprintf(ghk.nssid,"********\0");
  sprintf(ghk.DisplayName,"TV Remote\0");
  
  for ( int  i = 0 ; i < MAX_PAD ; i++ ){
    sprintf(IRPad[i].Display,"\0");
    IRPad[i].lCode = 0 ;
    IRPad[i].lRxCode = 0 ;
    IRPad[i].lBGColor = 0 ;
    IRPad[i].wRxTxType = 0 ;
  }
  sprintf(IRPad[0].Display,"ON/OFF\0");
  IRPad[0].lCode = TVONOFF ;
  IRPad[0].lRxCode = 0x18E710EF ;
  sprintf(IRPad[1].Display,"INFO\0");
  IRPad[1].lCode = TVINFO ;
  sprintf(IRPad[2].Display,"MUTE\0");
  IRPad[2].lCode = TVMUTE ;
  IRPad[2].lRxCode = 0xD88C247 ;
  sprintf(IRPad[3].Display,"AV\0");
  IRPad[3].lCode = TVAV ;
  IRPad[3].lRxCode = 0x18E750AF ;
  sprintf(IRPad[7].Display,"Vol+\0");
  IRPad[7].lCode = TVVOLUP ;
  IRPad[7].lRxCode = 0x18E740BF ;
  sprintf(IRPad[11].Display,"Vol-\0");
  IRPad[11].lCode = TVVOLDN ;
  IRPad[11].lRxCode = 0x18E7C03F ;
  sprintf(IRPad[6].Display,"CH+\0");
  IRPad[6].lCode = TVCHUP ;
  IRPad[6].lRxCode = 0x18E700FF ;
  sprintf(IRPad[10].Display,"CH-\0");
  IRPad[10].lCode = TVCHDN ;
  IRPad[10].lRxCode = 0x18E7807F ;

  sprintf(IRPad[8].Display,"MENU\0");
  IRPad[8].lCode = TVAPMENU ;
  IRPad[8].lRxCode = 0 ;
  sprintf(IRPad[9].Display,"UP\0");
  IRPad[9].lCode = TVUNAV ;
  IRPad[9].lRxCode = 0xD08A1A4 ;
  sprintf(IRPad[13].Display,"OK\0");
  IRPad[13].lCode = TVOKNAV ;
  IRPad[13].lRxCode = 0xD084144 ;
  sprintf(IRPad[17].Display,"DOWN\0");
  IRPad[17].lCode = TVDNAV ;
  IRPad[17].lRxCode = 0xD086164 ; 
  sprintf(IRPad[12].Display,"LEFT\0");
  IRPad[12].lCode = TVLNAV ;
  IRPad[12].lRxCode = 0xD08E1E4 ;
  sprintf(IRPad[14].Display,"RIGHT\0");
  IRPad[14].lCode = TVRNAV ;
  IRPad[14].lRxCode = 0xD081114 ;  
  sprintf(IRPad[19].Display,"EXIT\0");
  IRPad[19].lCode = TVEXITNAV ;

 
  sprintf(IRPad[24].Display,"1\0");
  IRPad[24].lCode = TV1PAD ;
  sprintf(IRPad[25].Display,"2\0");
  IRPad[25].lCode = TV2PAD ;
  sprintf(IRPad[26].Display,"3\0");
  IRPad[26].lCode = TV3PAD ;
  sprintf(IRPad[28].Display,"4\0");
  IRPad[28].lCode = TV4PAD ;
  sprintf(IRPad[29].Display,"5\0");
  IRPad[29].lCode = TV5PAD ;
  sprintf(IRPad[30].Display,"6\0");
  IRPad[30].lCode = TV6PAD ;
  sprintf(IRPad[32].Display,"7\0");
  IRPad[32].lCode = TV7PAD ;
  sprintf(IRPad[33].Display,"8\0");
  IRPad[33].lCode = TV8PAD ;
  sprintf(IRPad[34].Display,"9\0");
  IRPad[34].lCode = TV9PAD ;
  sprintf(IRPad[37].Display,"0\0");
  IRPad[37].lCode = TV0PAD ;

  sprintf(IRPad[40].Display,"PLAY\0");
  IRPad[40].lCode = TVPLAY ;
  sprintf(IRPad[41].Display,"PAUSE\0");
  IRPad[41].lCode = TVPAUSE ;
  sprintf(IRPad[42].Display,"STOP\0");
  IRPad[42].lCode = TVSTOP ;
  sprintf(IRPad[43].Display,"WTF\0");
  IRPad[43].lCode = TVWTF ;

  sprintf(IRPad[44].Display,"REW\0");
  IRPad[44].lCode = TVREW ;
  sprintf(IRPad[45].Display,"FF\0");
  IRPad[45].lCode = TVFF ;
  sprintf(IRPad[46].Display,"PREV\0");
  IRPad[46].lCode = TVPREV ;
  sprintf(IRPad[47].Display,"NEXT\0");
  IRPad[47].lCode = TVNEXT ;

}
