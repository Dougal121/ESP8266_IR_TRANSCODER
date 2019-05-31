void BackInTheBoxMemory(){
  sprintf(ghk.NodeName,"Control_%08X\0",ESP.getChipId());  
  sprintf(ghk.nssid,"lodge\0" ) ;
  sprintf(ghk.npassword,"\0" ) ;
  sprintf(ghk.cssid,"Control_%08X\0",ESP.getChipId());  
  sprintf(ghk.cpassword,"\0" ) ;
//  ghk.lDisplayOptions = 0 ; 
  ghk.lVersion = MYVER ;       
  ghk.lTxDelay = 50 ;
  ghk.butW = 75 ;
  ghk.butH = 50 ;
  ghk.butC = 4 ;
  ghk.lDefButBGColor = 0xB2FF66 ;      // olivy color WTF 
  ghk.lDefButTextColor = 0x000000 ;    // black
  ghk.MyIP = IPAddress(192,168,1,111) ;                                  // main
  ghk.MyIPC = IPAddress(192, 168, (5 +(ESP.getChipId() & 0x7f )) , 1) ;  // configuration       
  sprintf(ghk.npassword,"MyPassword\0");
  sprintf(ghk.nssid,"MySSID\0");
  
  sprintf(ghk.DisplayName,"Living Room\0");
  
  for ( int  j = 0 ; j < MAX_VR ; j++ ){
    for ( int  i = 0 ; i < MAX_PAD ; i++ ){
      sprintf(IRPad[j].pb[i].Display,"\0");
      IRPad[j].pb[i].lCode = 0 ;
      IRPad[j].pb[i].lRxCode = 0 ;
      IRPad[j].pb[i].lBGColor = 0 ;
      IRPad[j].pb[i].wRxTxType = 0 ;
    }
    sprintf(IRPad[j].Display,"\0");
    IRPad[j].lBGColor = 0xCCCC00 ;  // darker yellow
    IRPad[j].lBGHColor = 0xFFFF00 ; // bright yellow
  }
  sprintf(IRPad[0].Display,"TV\0");
  sprintf(IRPad[1].Display,"DVR\0");
  sprintf(IRPad[2].Display,"VCR\0");
  
  sprintf(IRPad[0].pb[0].Display,"ON/OFF\0");
  IRPad[0].pb[0].lCode = TVONOFF ;
  IRPad[0].pb[0].lRxCode = 0x18E710EF ;
  IRPad[0].pb[0].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[1].Display,"INFO\0");
  IRPad[0].pb[1].lCode = TVINFO ;
  IRPad[0].pb[1].lRxCode = 0xd8842c7 ;
  IRPad[0].pb[1].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[2].Display,"MUTE\0");
  IRPad[0].pb[2].lCode = TVMUTE ;
  IRPad[0].pb[2].lRxCode = 0xD88C247 ;
  IRPad[0].pb[2].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[3].Display,"AV\0");
  IRPad[0].pb[3].lCode = TVAV ;
  IRPad[0].pb[3].lRxCode = 0x18E750AF ;
  IRPad[0].pb[3].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[7].Display,"Vol+\0");
  IRPad[0].pb[7].lCode = TVVOLUP ;
  IRPad[0].pb[7].lRxCode = 0x18E740BF ;
  IRPad[0].pb[7].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[11].Display,"Vol-\0");
  IRPad[0].pb[11].lCode = TVVOLDN ;
  IRPad[0].pb[11].lRxCode = 0x18E7C03F ;
  IRPad[0].pb[11].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[6].Display,"CH+\0");
  IRPad[0].pb[6].lCode = TVCHUP ;
  IRPad[0].pb[6].lRxCode = 0x18E700FF ;
  IRPad[0].pb[6].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[10].Display,"CH-\0");
  IRPad[0].pb[10].lCode = TVCHDN ;
  IRPad[0].pb[10].lRxCode = 0x18E7807F ;
  IRPad[0].pb[10].wRxTxType = PAN2NEC ;

  sprintf(IRPad[0].pb[8].Display,"MENU\0");
  IRPad[0].pb[8].lCode = TVAPMENU ;
  IRPad[0].pb[8].lRxCode = 0 ;
  IRPad[0].pb[8].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[9].Display,"UP\0");
  IRPad[0].pb[9].lCode = TVUNAV ;
  IRPad[0].pb[9].lRxCode = 0xD08A1A4 ;
  IRPad[0].pb[9].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[13].Display,"OK\0");
  IRPad[0].pb[13].lCode = TVOKNAV ;
  IRPad[0].pb[13].lRxCode = 0xD084144 ;
  IRPad[0].pb[13].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[17].Display,"DOWN\0");
  IRPad[0].pb[17].lCode = TVDNAV ;
  IRPad[0].pb[17].lRxCode = 0xD086164 ; 
  IRPad[0].pb[17].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[12].Display,"LEFT\0");
  IRPad[0].pb[12].lCode = TVLNAV ;
  IRPad[0].pb[12].lRxCode = 0xD08E1E4 ;
  IRPad[0].pb[12].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[14].Display,"RIGHT\0");
  IRPad[0].pb[14].lCode = TVRNAV ;
  IRPad[0].pb[14].lRxCode = 0xD081114 ;  
  IRPad[0].pb[14].wRxTxType = PAN2NEC ;
  sprintf(IRPad[0].pb[19].Display,"EXIT\0");
  IRPad[0].pb[19].lCode = TVEXITNAV ;
  IRPad[0].pb[19].wRxTxType = NECONLY ;
 
  sprintf(IRPad[0].pb[20].Display,"1\0");
  IRPad[0].pb[20].lCode = TV1PAD ;
  IRPad[0].pb[20].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[21].Display,"2\0");
  IRPad[0].pb[21].lCode = TV2PAD ;
  IRPad[0].pb[21].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[22].Display,"3\0");
  IRPad[0].pb[22].lCode = TV3PAD ;
  IRPad[0].pb[22].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[24].Display,"4\0");
  IRPad[0].pb[24].lCode = TV4PAD ;
  IRPad[0].pb[24].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[25].Display,"5\0");
  IRPad[0].pb[25].lCode = TV5PAD ;
  IRPad[0].pb[25].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[26].Display,"6\0");
  IRPad[0].pb[26].lCode = TV6PAD ;
  IRPad[0].pb[26].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[28].Display,"7\0");
  IRPad[0].pb[28].lCode = TV7PAD ;
  IRPad[0].pb[28].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[29].Display,"8\0");
  IRPad[0].pb[29].lCode = TV8PAD ;
  IRPad[0].pb[29].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[30].Display,"9\0");
  IRPad[0].pb[30].lCode = TV9PAD ;
  IRPad[0].pb[30].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[33].Display,"0\0");
  IRPad[0].pb[33].lCode = TV0PAD ;
  IRPad[0].pb[33].wRxTxType = NECONLY ;

  sprintf(IRPad[0].pb[36].Display,"PLAY\0");
  IRPad[0].pb[36].lCode = TVPLAY ;
  IRPad[0].pb[36].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[37].Display,"PAUSE\0");
  IRPad[0].pb[37].lCode = TVPAUSE ;
  IRPad[0].pb[37].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[38].Display,"STOP\0");
  IRPad[0].pb[38].lCode = TVSTOP ;
  IRPad[0].pb[38].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[39].Display,"WTF\0");
  IRPad[0].pb[39].lCode = TVWTF ;
  IRPad[0].pb[39].wRxTxType = NECONLY ;

  sprintf(IRPad[0].pb[40].Display,"REW\0");
  IRPad[0].pb[40].lCode = TVREW ;
  IRPad[0].pb[40].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[41].Display,"FF\0");
  IRPad[0].pb[41].lCode = TVFF ;
  IRPad[0].pb[41].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[42].Display,"PREV\0");
  IRPad[0].pb[42].lCode = TVPREV ;
  IRPad[0].pb[42].wRxTxType = NECONLY ;
  sprintf(IRPad[0].pb[43].Display,"NEXT\0");
  IRPad[0].pb[43].lCode = TVNEXT ;
  IRPad[0].pb[43].wRxTxType = NECONLY ;


  // SECOND PAD
  sprintf(IRPad[1].pb[0].Display,"ON/OFF\0");
  IRPad[1].pb[0].lCode = TVONOFF ;
  IRPad[1].pb[0].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[1].Display,"INFO\0");
  IRPad[1].pb[1].lCode = TVINFO ;
  IRPad[1].pb[1].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[2].Display,"MUTE\0");
  IRPad[1].pb[2].lCode = TVMUTE ;
  IRPad[1].pb[2].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[3].Display,"AV\0");
  IRPad[1].pb[3].lCode = TVAV ;
  IRPad[1].pb[3].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[7].Display,"Vol+\0");
  IRPad[1].pb[7].lCode = TVVOLUP ;
  IRPad[1].pb[7].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[11].Display,"Vol-\0");
  IRPad[1].pb[11].lCode = TVVOLDN ;
  IRPad[1].pb[11].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[6].Display,"CH+\0");
  IRPad[1].pb[6].lCode = TVCHUP ;
  IRPad[1].pb[6].wRxTxType = PANONLY ;
  sprintf(IRPad[1].pb[10].Display,"CH-\0");
  IRPad[1].pb[10].lCode = TVCHDN ;
  IRPad[1].pb[10].wRxTxType = PANONLY ;


}
