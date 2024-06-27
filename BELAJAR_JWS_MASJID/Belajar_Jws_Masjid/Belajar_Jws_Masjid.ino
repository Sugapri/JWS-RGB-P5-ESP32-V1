/*************************************************************************************
  Waktu Sholat P10   - Program jam petunjuk waktu Sholat otomatis yang dilekapi dengan
  Copyright (C) Des 2017 MFH Robotic. (info https://www.facebook.com/MFH.Robotic/)

  Fitur :
  1. Perhitungan waktu sholat otomatis berdasarkan data geografis lokasi
    (Latitude, Longitude, Altitude, Time Zone dan Prameter pengaman/
    Ihtiyati)
  2. Fasilitas remote menggunakan aplikasi yang berjalan pada HP Android.
  3. Fasilitas running text informasi yang berjumlah 3 x 150 karakter.
    Running text ini cukup besar untuk di isi dengan hadis atau informasi lainnya
    melalui fasilitas remote Aplikasi Android.
  4. Perhitungan Hari dan tanggal Hijriah secara otomatis dan perubahan tanggalnya
    sesuai sesuai standar Hijriah yaitu setelah azzan magrib
  5. Fasilitas nama Masjid dan jenis masjid nya:
        a. Masjid
        b. Musholla
        c. Surau
        d. Langgar
  6. Pengingat waktu sholat mulai dari Azzan, menunggu iqomah dan mulai Sholat
    termasuk pengingat sholat Jumat jika parameter jenis nya di set sebagai Masjid.
  7. Program ini sangat baik dipakai sebagai sarana belajar programing Arduino karena
    menggunakan banyak librari dan teknik pemrogramn yang asik seperti:
       - Library yang digunakan adalah:
              #include <SPI.h>            --> komunikasi ke modul P10
              #include <DMD3.h>           --> library untuk modul P10
              #include <Wire.h>           --> komunikasi ke modul RTC DS3231
              #include <DS3231.h>         --> library modul RTC DS 3231
              #include <EEPROM.h>         --> library untuk penggunaan EEPROM sebagai sarana penyimpan parameter
              #include <avr/pgmspace.h>   --> library untuk penggunaan PROGMEM agar utilisasi SRAM bisa optimal
              #include "RGBmatrixPanel.h" --> https://github.com/adafruit/RGB-matrix-Panel
              #include "Adafruit_GFX.h"   --> https://github.com/adafruit/Adafruit-GFX-Library
  8. Program dipecah menjadi 4 File sehingga maintenance bisa lebih mudah dan aman dan tidak mengganggu bagian
    lain.

  Created by Wardi Utari
  di gabungkan dengan sketch jws pak Rokhmad
  30 Des 2017

  ---

  Progarm ini TIDAK ROYALTY FREE, program ini FREE jika dipergunakan untuk
    1. Jika dipergunakan di sekolah/lembaga latihan/perorangan untuk pendidikan atau proses belajar
    2. Dipergunakan dalam proyek pembuatan Jam pengingat waktu sholat yang disumbangkan ke masjid, musholla dll.

  Jika program ini dipergunakan secara komersial baik keseluruhan atau sebagian, anda diwajibkan
  membayar ROYALTY FEE sebesar 2.5%. Royalty tersebut agar dibayarkan ke Masjid terdekat dengan lokasi
  anda, dalam bentuk Sedekah yang diniatkan untuk semua orang yang telah turut urun rembug dalam membuat dan
  menyempurnakan program ini.

  Semoga Bermanfaat
  Salam Mujahid 212.
  Salam Bintang 9 .

  edit : rozak anugrah

**************************************************************************************/







#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <SPI.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <Adafruit_GFX.h>
RtcDS3231<TwoWire> Rtc(Wire);
#include <EEPROM.h>
#include <Fonts/BigNumber.h> //ok
#include <Fonts/Font5x7.h> //ok
#include <Fonts/Angka_6x15.h>
#include "Fonts/Tahoma8.h"
#include "Fonts/Tahoma5.h"
#include <Fonts/Digital4x7B.h>

#define PANEL_WIDTH 64 // Number of pixels wide of each INDIVIDUAL panel module. 
#define PANEL_HEIGHT 32 // Number of pixels tall of each INDIVIDUAL panel module.
#define PANEL_NUMBER 1
#define PANEL_WIDTH PANEL_WIDTH * PANEL_NUMBER
#define PANE_HEIGHT PANEL_HEIGHT
MatrixPanel_I2S_DMA *Disp = nullptr;

#define BUZZ 32
RtcDateTime now;
char weekDay[][7] = {"AHAD", "SENIN", "SELASA", "RABU", "KAMIS", "JUM'AT", "SABTU", "AHAD"}; 
char monthYear2[][5] = { "DES", "JAN", "FEB", "MAR", "APR", "MEI", "JUN", "JUL", "AGT", "SEP", "OKT", "NOV", "DES" };
#define EH          1948439.5f 
#define KH   0


struct TanggalDanWaktu
{
  uint8_t detik;
  uint8_t menit;
  uint8_t jam;
  uint8_t hari;
  uint8_t tanggal;
  uint8_t bulan;
  uint8_t tahun;
};

struct TanggalE
{
  uint8_t tanggal;
  uint8_t bulan;
  uint16_t tahun;
};

struct TanggalJawa
{
  uint8_t pasaran;
  uint8_t wuku;
  uint8_t tanggal;
  uint8_t bulan;
  uint16_t tahun;
};
struct JamDanMenit
{
  uint8_t jam;
  uint8_t menit;
};
uint8_t jumlahHariPerBulan[] = { 31, 28, 31, 30, 31, 30, 31, 31,30, 31, 30, 31};
char namaBulanMasehi[][10] = {"Januari", "Februari", "Maret","April", "Mei", "Juni", "Juli", "Agustus", "September", "Oktober","November", "Desember"};
char namaHariMasehi[][7] = {"Minggu", "Senin", "Selasa", "Rabu","Kamis", "Jum'at", "Sabtu",};
char namaBulanHijriah[][14] = {"Muharram", "Safar", "R.awal","R.Akhir", "J.Awal", "J.Akir", "Rajab","Sya'ban", "Ramadhan", "Syawal","D.kaidah", "D.hijjah"};
char namaBulanJawa[][12] = { "Sura", "Sapar", "Mulud", "Bakda Mulud", "Jumadilawal", "Jumadilakir", "Rejeb", "Ruwah", "Pasa", "Sawal", "Sela", "Besar"};
char namaHariPasaran[][7] = {"LEGI", "PAHING", "PON", "WAGE", "KLIWON"};

TanggalDanWaktu tanggalMasehi;
TanggalE tanggalHijriah;
TanggalJawa tanggalJawa;
JamDanMenit waktuMagrib;


WiFiServer server(137);
WiFiClient client;
const char *ssid = "AL_BAROKAH_JWS";
const char *pw   = "123456789";  
IPAddress ip(11, 11, 11, 11);
IPAddress netmask(255, 255, 255, 0);
typedef struct
  {
    uint8_t stt;
    float latitude;
    float longitude;
    float altitude;
    float zonawaktu;
    uint8_t kecerahan;
    uint8_t ihyati;
    uint8_t iqom1; 
    uint8_t iqom4;
    uint8_t iqom5;
    uint8_t iqom6;
    uint8_t iqom7;
    uint8_t k1;
    uint8_t k4;
    uint8_t k5;
    uint8_t k6;
    uint8_t k7;
    uint8_t ch;
    uint8_t buzzer;
    uint8_t kecepatan;
    uint8_t kecepatan_jadwal;
    uint8_t SO;      // 1 byte  add 20
    uint8_t JM;      // 1 byte  add 21
    uint8_t MT;      // 1 byte  add 21
    uint8_t jumlah_panel;
    
  } struct_prm;

struct_prm  Prm;
float sholatT[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t Iqomah[8] = {0, 0, 0, 0, 0, 0, 0, 0};
float floatnow = 0;int8_t SholatNow = -1;boolean azzan    = false;
uint8_t daynow = 0;boolean jumat    = false;uint8_t reset_x = 0;
int         RunSel    = 1;int         RunFinish = 0 ;
char imsak [6];char subuh [6];char terbit[6];char duha  [6];
char duhur [6];char asar  [6];char magrib[6];char isya  [6];
unsigned char jame,menite,detike,detikLalu,menitLalu;
unsigned char tanggale,bulane,tahune,harie;

uint16_t ABU    = Disp->color565(64, 64, 64);
uint16_t PUTIH  = Disp->color565(255, 255, 255);
uint16_t MERAH  = Disp->color565(255, 0, 0);
uint16_t KUNING = Disp->color565(255, 255, 0);
uint16_t BIRU   = Disp->color565(0, 0, 255);
uint16_t HIJAU  = Disp->color565(0, 255, 0);
uint16_t HITAM  = Disp->color565(0, 0, 0);
uint16_t CIAN = Disp->color565(0, 255, 255);
uint16_t PINK = Disp->color565(255, 0, 255);
uint16_t KUNING_TUA = Disp->color565(246, 214, 4);
uint16_t PINK_MUDA =  Disp->color565(242, 142, 130);
uint16_t UNGU = Disp->color565(158, 0, 179);
uint16_t CUSTOM = Disp->color565(139, 137, 255);
void firstRtc()
  {
  unsigned int temp_int;
  RtcDateTime now = Rtc.GetDateTime();

  temp_int=now.Year()-2000;
  tahune=temp_int;
  bulane=now.Month();
  tanggale=now.Day();
  harie=now.DayOfWeek();
  jame=now.Hour();
  menite=now.Minute();
  detike=now.Second(); 
  }



void setup() {
  int rtn = I2C_ClearBus(); // clear the I2C bus first before calling Wire.begin()
  if (rtn != 0) {
    if (rtn == 1) {
    } else if (rtn == 2) {
    } else if (rtn == 3) {
    }
  } else { // bus clear, re-enable Wire, now can start Wire Arduino master
    Wire.begin();
  }

  Rtc.Begin();

  if (!Rtc.GetIsRunning()) {
    Rtc.SetIsRunning(true);
  }

   Rtc.Enable32kHzPin(false);
   Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
   Serial.println("Setup RTC selesai");
   
   Serial.begin(9600);
   EEPROM.begin(4096);
   firstRtc();
   WiFi.mode(WIFI_AP);
   WiFi.softAPConfig(ip, ip, netmask);
   WiFi.softAP(ssid, pw);
   server.begin(); 
   HUB75_I2S_CFG mxconfig;mxconfig.mx_height = PANEL_HEIGHT;mxconfig.chain_length = PANEL_NUMBER;
   mxconfig.double_buff = true; 
   mxconfig.clkphase = false;
   Disp = new MatrixPanel_I2S_DMA(mxconfig);Disp->setBrightness8(Prm.kecerahan);
    if( not Disp->begin() )
      Serial.println("****** !KABOOM! I2S memory allocation failed ***********");
 
 Disp->clearScreen();

SavePrm();
updateTime(); 
SendCon();
    pinMode(BUZZ,OUTPUT);
    digitalWrite(BUZZ, HIGH);
    delay(100);
    digitalWrite(BUZZ, LOW);
    delay(100); 
    digitalWrite(BUZZ, HIGH);
    delay(100);
    digitalWrite(BUZZ, LOW);
    delay(100);   
}

void loop() {
  SerialWifi();
  updateTime();
  Timer_Minute(1);
  Disp->flipDMABuffer();
  scan();
  matrix_init();
  check_azzan();
  informasi(100,50,JamBesar,1);//NAMA MASJID
  informasi(250,50,JamBesar,2);
  informasi(505,50,JamBesar,3);
  informasi(760,50,JamBesar,4);
  
  informasi(1270,40,JamBesar,102); 
  informasi(1370,40,JamBesar,103);


  drawAzzan(100);
  drawIqomah1(101);
  blinkBlock(104);

  if (RunFinish == 1) {RunSel =2; RunFinish = 0;}  
  if (RunFinish == 2) {RunSel =3; RunFinish = 0;}  
  if (RunFinish == 3) {RunSel =4; RunFinish = 0;} 
  if (RunFinish == 4) {RunSel =1; RunFinish = 0;} 

 if(RunFinish ==99) {RunSel =100; RunFinish = 0;}
 if (RunFinish == 100 and jumat ) {RunSel = 103; RunFinish = 0; reset_x = 1;}else
 if (RunFinish == 100) { RunSel = 101; RunFinish = 0;}
 if (RunFinish == 101) {RunSel = 102; RunFinish = 0; reset_x = 1;}
 if (RunFinish == 102) {RunSel = 104; RunFinish = 0;}
 if (RunFinish == 103) {RunSel = 104; RunFinish = 0;}
 if (RunFinish == 104) {RunSel = 1;  RunFinish = 0;}
  
}


void matrix_init(){Disp->setBrightness8(Prm.kecerahan); Disp->clearScreen();}
void scan(){
      static uint16_t   lsRn;
      uint16_t     Tmr = millis();
    if ((int)now.Minute() == 0){   
        Disp->fillScreen(0);
  }
}

void updateTime()
{ RtcDateTime now = Rtc.GetDateTime();
  floatnow = (float)now.Hour() + (float)now.Minute() / 60 + (float)now.Second() / 3600;
  daynow   = now.DayOfWeek();    // load day Number
}

void Timer_Minute(int repeat_time) //load every  1 minute
{
  static uint16_t   lsRn;
  uint16_t          Tmr = millis();
  if ((Tmr - lsRn) > (repeat_time * 60000))
  {
    lsRn = Tmr;
    update_All_data();
  }
}


void update_All_data()
{
  uint8_t   date_cor = 0;
  updateTime();
  sholatCal();                                                // load Sholah Time
  islam();

   if ((floatnow > (float)23) or (floatnow < (float)3.5) )  {Disp->setBrightness8(10);}
    else                                                   {Disp->setBrightness8(Prm.kecerahan);}
}
