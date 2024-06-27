
// Koneksi Remot 
char CH_Prm[255];
void SerialWifi()
  {
    
    if (!client.connected())
        {
          client = server.available();
          return;
        }

    int prm_idx = 0;
    char bchar;
    bchar = client.peek();

    if ((bchar == 'X') or (bchar == 'Y') or (bchar == 'Z'))
        {
          while ((bchar != '\n') and (prm_idx < 249))
            {
              if (client.available())
                {
                  bchar = (char)client.read();
                  CH_Prm[prm_idx]=bchar;
                  prm_idx ++;
                  digitalWrite(BUZZ, HIGH);
                  delay(20);
                  digitalWrite(BUZZ, LOW);
                }
            }
            CH_Prm[prm_idx]='\0';
            OpenCon();
            SendCon();
        }
        else
        {
          while (client.available())
            {
              client.write(client.read());
              Serial.write(client.read());
              
            }
           digitalWrite(BUZZ,LOW); 
        }
  }


void OpenCon()
  {
    String wifi;
    uint16_t eeadd = 0;
    uint8_t  eeput = 0;
    uint8_t  eemax = 0;
    uint8_t lenprm = strlen(CH_Prm)-3;
    int addr = 0;

    if (CH_Prm[0]=='X')
      {
        if(CH_Prm[1]=='N' and CH_Prm[2] =='M') 
          {
            eeadd = 100;
            eemax = 149;
          }
      
      else
      if (CH_Prm[1]=='N' and CH_Prm[2] =='1')
          {
            eeadd = 250;
            eemax = 254;
          }
      else
      if (CH_Prm[1] == 'N' and CH_Prm[2] =='2')
          {
            eeadd = 505;
            eemax = 254;
          }
     else
     if (CH_Prm[1] == 'N' and CH_Prm[2] =='3')
          {
            eeadd = 760;
            eemax = 254;
          }
    else 
    if (CH_Prm[1] =='N' and CH_Prm[2] == '4')
          {
            eeadd = 1015;
            eemax = 255;
          }

    else 
    if (CH_Prm[1] =='S' and CH_Prm[2] == 'M')
          {
            eeadd = 1270;
            eemax = 99;
          }
   else 
    if (CH_Prm[1] =='J' and CH_Prm[2] == 'M')
          {
            eeadd = 1370;
            eemax = 99;
          }
  int eecount = 0;
  while ((eecount <= lenprm) and (eecount < eemax))
        {
          EEPROM.write(eeadd+eecount, CH_Prm[3+eecount]);
          eecount++;
         // EEPROM.commit();
        }
        EEPROM.write(eeadd+eecount,'\0');
        EEPROM.commit();
  }


  else if (CH_Prm[0] == 'Y')
          {
            wifi = String(CH_Prm);
            if (wifi.substring(1, 3).equals("LA")) {eeadd = 1; eeput=1;}
            else if(wifi.substring(1, 3).equals("LO")) {eeadd = 5; eeput=1;}
            else if(wifi.substring(1, 3).equals("AL")) {eeadd = 9; eeput=1;}
            else if(wifi.substring(1, 3).equals("TZ")) {eeadd = 13; eeput=1;}
            else if(wifi.substring(1, 3).equals("BL")) {eeadd = 17; eeput=2;}//kecerahan
            else if(wifi.substring(1, 3).equals("IH")) {eeadd = 18; eeput=2;}//ihyati
            else if(wifi.substring(1, 3).equals("CH")) {eeadd = 19; eeput=2;}//koreksi hijriah
            else if(wifi.substring(1, 3).equals("BZ")) {eeadd = 20; eeput=2;}//Buzzer
            else if(wifi.substring(1, 3).equals("S1")) {eeadd = 21; eeput=2;}//kecepatan Text1
            else if(wifi.substring(1, 3).equals("S2")) {eeadd = 22; eeput=2;}//kecepatan Text2
            else if(wifi.substring(1, 3).equals("SO")) {eeadd = 23; eeput=2;}//Lama Sholat
            else if(wifi.substring(1, 3).equals("SM")) {eeadd = 24; eeput=2;}// LamaJumat
            else if(wifi.substring(1, 3).equals("JP")) {eeadd = 25; eeput=2;}//Jumlah Panel
            else if(wifi.substring(1, 3).equals("I1")) {eeadd = 26; eeput=2;}//iqomah subuh
            else if(wifi.substring(1, 3).equals("I4")) {eeadd = 27; eeput=2;}//iqomah dzuhur
            else if(wifi.substring(1, 3).equals("I5")) {eeadd = 28; eeput=2;}// iqomah ashar
            else if(wifi.substring(1, 3).equals("I6")) {eeadd = 29; eeput=2;}// iqomah Magrib
            else if(wifi.substring(1, 3).equals("I7")) {eeadd = 30; eeput=2;}// iqomah isya
            else if(wifi.substring(1, 3).equals("K1")) {eeadd = 31; eeput=2;}// Koreksi subuh
            else if(wifi.substring(1, 3).equals("K4")) {eeadd = 32; eeput=2;}// Koreksi Dzuhur
            else if(wifi.substring(1, 3).equals("K5")) {eeadd = 33; eeput=2;}// Koreksi Ashar
            else if(wifi.substring(1, 3).equals("K6")) {eeadd = 34; eeput=2;}// Koreksi Maghrib
            else if(wifi.substring(1, 3).equals("K7")) {eeadd = 35; eeput=2;}// Koreksi Isya
            else if(wifi.substring(1, 3).equals("SL")) {eeadd = 66; eeput=2;}
            else if(wifi.substring(1, 3).equals("SJ")) {eeadd = 99; eeput=2;}          
            else if(wifi.substring(1, 3).equals("MT")) {eeadd = 96; eeput=1;}
          

 if  (eeput == 1)
      {
        EEPROM.put(eeadd, (float)wifi.substring(3, wifi.length()).toFloat());
        EEPROM.commit();
      }
 if  ( eeput == 2)
     {
      EEPROM.put(eeadd, (uint8_t)wifi.substring(3, wifi.length()).toInt());
      EEPROM.commit();
     }

  if   ( eeput == 3)
     {
      EEPROM.put(eeadd, (uint8_t)wifi.substring(4, wifi.length()).toInt());
      EEPROM.commit();
     }
    EEPROM.commit();  
  }

else
if (CH_Prm[0] == 'Z' and CH_Prm[1] =='D' and CH_Prm[2] =='T')
    {
      wifi = String(CH_Prm);
      uint16_t yer=(byte(wifi.substring(7,9).toInt()));
      uint8_t  mon=(byte(wifi.substring(5,7).toInt()));
      uint8_t  dat=(byte(wifi.substring(3,5).toInt()));
      uint8_t  hor(byte(wifi.substring(9,11).toInt()));
      uint8_t  minu=(byte(wifi.substring(11,13).toInt()));
      uint8_t  seco=(byte(wifi.substring(13,15).toInt()));
      Rtc.SetDateTime(RtcDateTime(yer,mon,dat,hor,minu,seco));
    }
SavePrm();
}// and OpenCon




void EEPROMWritefloat(int ee, float vl)
  {
    byte* p = (byte*)(void*)&vl;
    for (int i = 0 ; i < sizeof(vl); i++)
        EEPROM.write(ee++, *p++);
        
  }

float EEPROMReadfloat(int ee)
{
   float value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return value;
}

uint8_t EEPROMReadint(int ee)
{
   uint8_t vl = 0.0;
   byte* p = (byte*)(void*)&vl;
   for (int i = 0; i < sizeof(vl); i++)
       *p++ = EEPROM.read(ee++);
   return vl;
}

String bacaEEPROMString(int addr, int length){
  String s;
  for (int i = 0; i < length; i++){
    char c = EEPROM.read(addr+i);
      if (c == 0){
        break;
      }
      s +=c;
  }
  return s;
}


void SavePrm()
{

     EEPROM.get(0, Prm);
    if(Prm.stt != 99) //check value of Prm State .. set Default  
      {
        set_default_prm();
      }
   

Prm.latitude   = EEPROMReadfloat(1);
Prm.longitude  = EEPROMReadfloat(5);
Prm.altitude   = EEPROMReadfloat(9);
Prm.zonawaktu  = EEPROMReadfloat(13);
Prm.kecerahan = EEPROMReadint(17);
Prm.ihyati    = EEPROMReadint(18);
Prm.ch        = EEPROMReadint(19);
Prm.buzzer    = EEPROMReadint(20);
Prm.kecepatan = EEPROMReadint(21);
Prm.kecepatan_jadwal = EEPROMReadint(22);
Prm.SO    = EEPROMReadint(23);
Prm.JM    = EEPROMReadint(24);
Prm.jumlah_panel = EEPROMReadint(25);
Iqomah[1] = EEPROMReadint(26);
Iqomah[4] = EEPROMReadint(27);
Iqomah[5] = EEPROMReadint(28);
Iqomah[6] = EEPROMReadint(29);
Iqomah[7] = EEPROMReadint(30);
Prm.k1 = EEPROMReadint(31);
Prm.k4 = EEPROMReadint(32);
Prm.k5 = EEPROMReadint(33);
Prm.k6 = EEPROMReadint(34);
Prm.k7 = EEPROMReadint(35);
Prm.MT = EEPROMReadint(96);
update_All_data();



}

void set_default_prm()
      {
        // Put Parameter start form addr 500
        Prm = (struct_prm){99,-7.9419,112.6227,510,7,150,1,5,5,5,5,5,0,0,0,0,0,-1,1,40,60,5,15,1};
        EEPROM.put(0,Prm);
        EEPROM.put(55, "BULAN PURNAMA\0");
        EEPROM.put(130,"Info 1\0");
        EEPROM.put(280,"Info 2\0");
        EEPROM.put(430,"Info 3\0");
        EEPROM.put(580,"Ayo Sholat\0");
        EEPROM.put(730,"Ayo Jumatan\0");

        EEPROMWritefloat(1, -7.9419);
        EEPROMWritefloat(5, 112.6227);
        EEPROMWritefloat(9, 510);
        EEPROMWritefloat(13, 7);

        EEPROM.commit();

      }

void set_default_time(){ Rtc.SetDateTime(RtcDateTime(20,01,01,12,0,0));}
      


void SendCon()
  {
  
    Serial.print(F("PARAMETER :")); Serial.print("\n");
    Serial.print(F("==============================")); Serial.print("\n");
    Serial.print(F("1. Stt  : ")); Serial.print(Prm.stt);Serial.print("\n");//latitude
    Serial.print(F("1. Latitude  : ")); Serial.print(Prm.latitude,7);Serial.print("\n");//latitude
    Serial.print(F("2. Longitude  : ")); Serial.print(Prm.longitude,7);Serial.print("\n");//longitude
    Serial.print(F("3. Altitude  : ")); Serial.print(Prm.altitude,7);Serial.print("\n");//altitude
    Serial.print(F("4. Time Zone  : ")); Serial.print(Prm.zonawaktu,0);Serial.print("\n");//time zone
    Serial.print(F("5. Ikhtiyati  : ")); Serial.print(Prm.ihyati);Serial.print("\n");//lama ikhtiyati    
    Serial.print(F("6. Lama Sholat  : ")); Serial.print(Prm.SO);Serial.print("\n");//lama sholat
    Serial.print(F("7. Lama Sholat Jum'at  : ")); Serial.print(Prm.JM);Serial.print("\n");// lama sholat jumat
    Serial.print(F("8.Kondisi Buzzer  : ")); Serial.print(Prm.buzzer);Serial.print("\n");// on/off buzze
    Serial.print(F("kecerahan  : "));Serial.print(Prm.kecerahan);Serial.print("\n");
    Serial.print(F("==============================")); Serial.print("\n");
    Serial.print(F("END OF PARAMETER :")); Serial.print("\n");

  }
  





// I2C_ClearBus menghindari gagal baca RTC (nilai 00 atau 165)

int I2C_ClearBus() {

#if defined(TWCR) && defined(TWEN)
  TWCR &= ~(_BV(TWEN)); //Disable the Atmel 2-Wire interface so we can control the SDA and SCL pins directly
#endif

  pinMode(SDA, INPUT_PULLUP); // Make SDA (data) and SCL (clock) pins Inputs with pullup.
  pinMode(SCL, INPUT_PULLUP);

  delay(2500);  // Wait 2.5 secs. This is strictly only necessary on the first power
  // up of the DS3231 module to allow it to initialize properly,
  // but is also assists in reliable programming of FioV3 boards as it gives the
  // IDE a chance to start uploaded the program
  // before existing sketch confuses the IDE by sending Serial data.

  boolean SCL_LOW = (digitalRead(SCL) == LOW); // Check is SCL is Low.
  if (SCL_LOW) { //If it is held low Arduno cannot become the I2C master.
    return 1; //I2C bus error. Could not clear SCL clock line held low
  }

  boolean SDA_LOW = (digitalRead(SDA) == LOW);  // vi. Check SDA input.
  int clockCount = 20; // > 2x9 clock

  while (SDA_LOW && (clockCount > 0)) { //  vii. If SDA is Low,
    clockCount--;
    // Note: I2C bus is open collector so do NOT drive SCL or SDA high.
    pinMode(SCL, INPUT); // release SCL pullup so that when made output it will be LOW
    pinMode(SCL, OUTPUT); // then clock SCL Low
    delayMicroseconds(10); //  for >5uS
    pinMode(SCL, INPUT); // release SCL LOW
    pinMode(SCL, INPUT_PULLUP); // turn on pullup resistors again
    // do not force high as slave may be holding it low for clock stretching.
    delayMicroseconds(10); //  for >5uS
    // The >5uS is so that even the slowest I2C devices are handled.
    SCL_LOW = (digitalRead(SCL) == LOW); // Check if SCL is Low.
    int counter = 20;
    while (SCL_LOW && (counter > 0)) {  //  loop waiting for SCL to become High only wait 2sec.
      counter--;
      delay(100);
      SCL_LOW = (digitalRead(SCL) == LOW);
    }
    if (SCL_LOW) { // still low after 2 sec error
      return 2; // I2C bus error. Could not clear. SCL clock line held low by slave clock stretch for >2sec
    }
    SDA_LOW = (digitalRead(SDA) == LOW); //   and check SDA input again and loop
  }
  if (SDA_LOW) { // still low
    return 3; // I2C bus error. Could not clear. SDA data line held low
  }

  // else pull SDA line low for Start or Repeated Start
  pinMode(SDA, INPUT); // remove pullup.
  pinMode(SDA, OUTPUT);  // and then make it LOW i.e. send an I2C Start or Repeated start control.
  // When there is only one I2C master a Start or Repeat Start has the same function as a Stop and clears the bus.
  /// A Repeat Start is a Start occurring after a Start with no intervening Stop.
  delayMicroseconds(10); // wait >5uS
  pinMode(SDA, INPUT); // remove output low
  pinMode(SDA, INPUT_PULLUP); // and make SDA high i.e. send I2C STOP control.
  delayMicroseconds(10); // x. wait >5uS
  pinMode(SDA, INPUT); // and reset pins as tri-state inputs which is the default state on reset
  pinMode(SCL, INPUT);
  return 0; // all ok

}
