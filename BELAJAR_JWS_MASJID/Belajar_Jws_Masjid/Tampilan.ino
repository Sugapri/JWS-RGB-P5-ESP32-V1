 // sholatN 9 x 8 
   const char static sholatN_E[] PROGMEM = {"IMSAK\0\0\0"
                                            "SHUBUH\0\0" 
                                            "TERBIT\0\0"  
                                            "DHUHA\0\0\0"   
                                            "DZUHUR\0\0"    
                                            "ASHAR\0\0\0"   
                                            "MAGRIB\0\0"    
                                            "ISYA'\0\0\0"
                                            "JUM'AT\0\0"
                                            };
char* sholatN(int number) // get sholat name from EEPROM
    {
      static char  locBuff[8];
      int locLen = number*8;
      memccpy_P(locBuff,sholatN_E+locLen  ,0,8);
      return locBuff;
    }



boolean dwDo(int DrawAdd)
      {
        if (RunSel == DrawAdd){return true;} 
        else {  return false;}
      }

void dwDone(int DrawAdd)
    { RunFinish = DrawAdd;RunSel = 0;}
    
void dwCtr(int x, int y, const char*Msg)
      { 
        
        int16_t x1, y1;
        uint16_t w = Disp->width(), h = Disp->height();    
        Disp->getTextBounds(Msg, x, y, &x1, &y1, &w, &h);
        Disp->setCursor(x-w/ 2, y);
        Disp->printf(Msg);
        
      }

      void jadwalSholat()
    {
      RtcDateTime now = Rtc.GetDateTime();
      static uint8_t x;
      static uint8_t y;
      static uint8_t jadwal;
      static uint32_t mJ;
      uint32_t  mL = millis();
      char jws[50];
      char wjs[50];


      if ((mL - mJ)> 50){
        if(x == 0 and y < 20){mJ = mL;y++;}
        if(x == 1 and y > 0 ){mJ = mL;y--;}
      }
      if ((mL - mJ)> 2000 and y == 20) {x=1;}
      if (y==0 and x == 1){jadwal++;x=0;}
      if (jadwal >=8){jadwal=0;}
     
      if (jadwal == 0){
        Disp->setFont(&Digital4x78pt7b);
        Disp->setTextColor(HIJAU);drawText(35,y-9,"IMSAK");
        Disp->setFont(&Font5x78pt7b);
        Disp->setTextColor(KUNING); 
        drawText(33, y-6,imsak); 
 }

      if (jadwal == 1){
        Disp->setFont(&Digital4x78pt7b);
        Disp->setTextColor(HIJAU);drawText(33, y-9,"SHUBUH");
        Disp->setFont(&Font5x78pt7b);
        Disp->setTextColor(KUNING);   
        drawText(33, y-6,subuh);      
      }
      if (jadwal == 2){
        
          Disp->setFont(&Digital4x78pt7b);
          Disp->setTextColor(HIJAU);   drawText(33, y-9,"TERBIT");       
           Disp->setFont(&Font5x78pt7b);
         Disp->setTextColor(KUNING); drawText(33, y-6,terbit);
        } 

        if (jadwal == 3){
           Disp->setFont(&Digital4x78pt7b);
           Disp->setTextColor(HIJAU); drawText(37, y-9,"DUHA");
           Disp->setFont(&Font5x78pt7b);
           Disp->setTextColor(KUNING);  
           drawText(33, y-6,duha);
       
        }
        
      if (jadwal == 4){
        Disp->setFont(&Digital4x78pt7b);
        Disp->setTextColor(HIJAU);drawText(33, y-9,"DZUHUR");
         Disp->setFont(&Font5x78pt7b);
        Disp->setTextColor(KUNING);
        drawText(33, y-6,duhur); 
 
      }
      
       if (jadwal == 5){
          Disp->setFont(&Digital4x78pt7b);
          Disp->setTextColor(HIJAU);drawText(33,y- 9,"ASHAR");
          Disp->setFont(&Font5x78pt7b);
          Disp->setTextColor(KUNING);
          drawText(33,y- 5,asar); 
         }
       
      if (jadwal == 6 ){
         Disp->setFont(&Digital4x78pt7b);
         Disp->setTextColor(HIJAU);drawText(33, y-9,"MAGRIB"); 
          Disp->setFont(&Font5x78pt7b);
         Disp->setTextColor(KUNING);
         drawText(33, y-6,magrib); 
     
       }
       
      if (jadwal == 7){
         Disp->setFont(&Digital4x78pt7b);
         Disp->setTextColor(HIJAU); drawText(33,y-9,"ISYA'");
          Disp->setFont(&Font5x78pt7b);
         Disp->setTextColor(KUNING);
         drawText(33, y-6,isya);
      }

  
     
 }

 
void drawText(uint16_t x, uint16_t y,const char *msg)
      {
        Disp->setCursor(x,y);
       // Disp->setTextColor(color);
        Disp->print(msg);
      }

 void informasi(int address,int speed,void menu(), int DrawAdd) {
  
   if (!dwDo(DrawAdd))return;
    static uint32_t x;  
    if (reset_x != 0) {x = 0; reset_x =0;}
  char data[255];
  for (int i = 0; i < 255; i++) {
    data[i] = EEPROM.read(address + i);
  }
  String text = String(data);
  static uint32_t y;
  int full = ((text.length()*8)+Disp->width());
  if((millis()-y)>speed){
    if(x<full){
    y=millis();
    ++x;
    }else{
      dwDone(DrawAdd);
      x=0;
    }
  }
   Animasi_tanggal();
   Disp->fillRect(0,0,64,16,HITAM);
   Disp->fillRect(0,24,64,9,HITAM);
   JamBesar();
  
   Disp->setFont(&Font5x78pt7b);
   Disp->setTextColor(KUNING);
   Disp->setCursor(Disp->width() - x,31);
   Disp->print(text.c_str());
  
   jadwalSholat();
    
}






 void blinkBlock(int DrawAdd)
  {
    RtcDateTime now = Rtc.GetDateTime();
  
    if(!dwDo(DrawAdd)) return;

    static uint32_t   lsRn;
    static uint16_t   ct, ct_l;
    int               mnt, scd;
    uint32_t          Tmr = millis();
    char              locBuff[6];

    if(jumat)       {ct_l = Prm.JM * 60;}
    else            {ct_l = Prm.SO * 60;}

     
    if((Tmr-lsRn)> 1000)
      {   lsRn=Tmr;
       
        ct++;
        }
        
   
    if (ct> ct_l) 
      { dwDone(DrawAdd);
        azzan = false;
        jumat = false;
        ct = 0;
      }


      
  }


void drawAzzan(int DrawAdd)
{
  RtcDateTime now = Rtc.GetDateTime();
  // check RunSelector
  if (!dwDo(DrawAdd)) return;
  uint8_t           ct_limit = 450; ///390 //harus angka genap
  static uint8_t    ct;
  static uint32_t   lsRn;
  uint32_t          Tmr = millis();

  if ((Tmr - lsRn) > 1000 and ct <= ct_limit)

  {
    lsRn = Tmr;
    //DoSwap = true;
    ct++;
  }
  if ((ct % 2) == 0)
  {
    Disp->setTextColor(PUTIH);
    Disp->setFont(&Tahoma5pt7b);
    Disp->setCursor(0,25);
    Disp->print(sholatN(SholatNow));
    
    if (ct <= 20)
    {
     digitalWrite(32, HIGH);
    }
  }
  else
  {
   digitalWrite(32, LOW);
  }
  
  Disp->setFont(&Tahoma8pt7b);
  Disp->setCursor(0,12);
  Disp->setTextColor(MERAH);
  Disp->print("ADZAN");



  if ((Tmr - lsRn) > 500 and (ct > ct_limit))
  { dwDone(DrawAdd);
    ct = 0;
   digitalWrite(32, LOW);
  }

}









void drawIqomah1(int DrawAdd)  // Countdown Iqomah (9 menit)
  {  
    // check RunSelector
     RtcDateTime now = Rtc.GetDateTime();
    if(!dwDo(DrawAdd)) return;

    static uint32_t   lsRn;
     static uint32_t pMKedip;
    uint32_t          Tmr = millis();
    static int        ct;
    int               mnt, scd,cn_l ;
    char              locBuff[6];           
    
       
 

    
    cn_l  = (Iqomah[SholatNow]*60);
    
    if((Tmr-lsRn) > 1000 and ct <=cn_l)
      {
          lsRn = Tmr;        
          if (ct> (cn_l-2)) digitalWrite(BUZZ,HIGH);   // Buzzer on 2 seccon before Iqomah
          ct++;
          // DoSwap = true;   
      }
          
          mnt = floor((cn_l-ct)/60); 
          scd = (cn_l-ct)%60;
          if(mnt>0) {
          Disp->setFont(&Tahoma5pt7b);sprintf(locBuff,"%02d:%02d",mnt,scd);Disp->setTextColor(HIJAU);drawText(15, 22,locBuff);}
          else {
          Disp->setFont(&Tahoma5pt7b);sprintf(locBuff,"%02d",scd);Disp->setTextColor(HIJAU);drawText(30, 22,locBuff);}    
          
    
               Disp->setFont(&Tahoma5pt7b);
               Disp->setTextColor(MERAH);
               drawText(9, 10,"IQOMAH");
            
    
    if (ct > cn_l)
      {
       
       dwDone(DrawAdd);
       ct = 0;
      digitalWrite(BUZZ,LOW);
       
      }
      
 
   
  }

void JamBesar(){
  RtcDateTime now = Rtc.GetDateTime();
  islam();
  static uint32_t mKedip;
  static boolean  kedip;
 
  char jam[3]; char menit[3]; char detik[3];
  char tanggaL[3];char tahuN[10];
  char out[20];char tgl[6];char tglh[3];
  
  
  sprintf(jam,"%02d",now.Hour());
  sprintf(menit,"%02d",now.Minute());
  sprintf(detik,"%02d",now.Second());
  
  if(now.Second()%2==0){
    Disp->drawCircle(16,4,1, HITAM);
    Disp->drawCircle(16,10,1,HITAM);
   
  } else {
    
    Disp->drawCircle(16,4,1, PINK);
    Disp->drawCircle(16,10,1,PINK);
  }
  Disp->setCursor(1,16);
  Disp->setFont(&Angka_6x15 ); 
  Disp->setTextColor(MERAH); Disp->print(jam);
  Disp->setCursor(19,16);
  Disp->setTextColor(MERAH);Disp->print(menit);
}

void Animasi_tanggal(){
  RtcDateTime now = Rtc.GetDateTime();
  islam();
  static uint32_t x;
  static uint32_t y;
  static uint8_t j;
  static uint32_t mT;
  char out[15];
  char thnH[6];
  sprintf(out, "%02d %s",tanggalHijriah.tanggal,namaBulanHijriah[tanggalHijriah.bulan - 1]); 
  sprintf(thnH, "%04dH", tanggalHijriah.tahun); 
  int width = Disp->width();
  //sprintf(hari,"%s %s %02d - %02d - %04d ",weekDay[now.DayOfWeek()],namaHariPasaran[tanggalJawa.pasaran],now.Day(),now.Month(),now.Year());
 // String Hari = hari;
 if ((millis()-mT)>30)
    {
      if(x == 0 and y <8)
        {
          mT = millis();
          y++;
        }    
    }
 if((millis()-mT)> 3000 and y == 8){x=1;}
 if( x == 1 and y >= 8)
    {mT=millis();
      y++;
      }
 if((millis()- mT )< 5000 and x == 1 and y == 32){
  j++;
  y=0;
 }
 if(y == 0 and x == 1){x=0;}
  Disp->setFont(&Font5x78pt7b);
 
 if(j>=4){j=0;}

 
 if(j==0){
  Disp->setCursor(0,30-y);
  Disp->setTextColor(CIAN); 
  Disp->print(weekDay[now.DayOfWeek()]);Disp->print(".");Disp->print(namaHariPasaran[tanggalJawa.pasaran]);
 }
  if(j==1){
    Disp->setCursor(3,30-y);
    Disp->setTextColor(PINK); 
    Disp->print(now.Day());
    Disp->print("-");
    Disp->print(now.Month());
    Disp->print("-");
    Disp->print(now.Year());
 }

 if(j == 2){
   Disp->setCursor(0,30-y);
   Disp->setTextColor(HIJAU); 
   Disp->print(out);
 }

  if(j == 3){
   Disp->setCursor(16,30-y);
   Disp->setTextColor(BIRU); 
   Disp->print(thnH);
 }

}

void check_azzan(){ 
  SholatNow  = -1;
  for (int i = 0; i <= 7; i++)
  {

    if (i != 0 and i != 2 and i != 3) // bukan terbit dan bukan dhuha
    {
if (floatnow >= sholatT[i])
      {
        SholatNow = i;
        if (!azzan and (floatnow > sholatT[i]) and (floatnow < (sholatT[i] + 0.03)))
        {
          if (daynow == 6 and SholatNow == 4 and Prm.MT == 1) {jumat = true;} 
          azzan = true;
          RunSel = 100;
        }
      }
      
    }
  }
  
}
 
