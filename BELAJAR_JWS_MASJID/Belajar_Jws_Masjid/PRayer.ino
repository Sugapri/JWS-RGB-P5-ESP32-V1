//------------------------------------------
// Function calculate Pray Time
//------------------------------------------
/*Macro Function */
#define d2r(x) x*M_PI/180
#define r2d(x) x*180/M_PI

//Constanta 
    const float lunarY          = 354.367068f;

// Main Function
void sholatCal()
  {
    float EJD = E_Julian_date(now.Year(),now.Month(),now.Day(),Prm.longitude);
    float Decl=Dql(EJD);
    float EqOfTime=EqT(EJD);
    Pray_Time(Prm.zonawaktu, Prm.latitude, Prm.longitude,Prm.altitude,Decl, EqOfTime );
      for (int i = 0;i < 8 ;i++)
      {
        float stime  = sholatT[i];
        uint8_t shour = floor(stime);
        uint8_t sminute = floor((stime - (float)shour) * 60);
        uint8_t ssecond = floor((stime - (float)shour - (float)sminute / 60) * 3600);
       switch (i)
    {
    case 0:
      sprintf(imsak, "%02d:%02d", shour, sminute);
      break;
    case 1:
      sprintf(subuh, "%02d:%02d", shour, sminute);
      break;
    case 2:
      sprintf(terbit, "%02d:%02d", shour, sminute);
      break;
    case 3:
      sprintf(duha, "%02d:%02d", shour, sminute);
      break;
    case 4:
      sprintf(duhur, "%02d:%02d", shour, sminute);
      break;
    case 5:
      sprintf(asar, "%02d:%02d", shour, sminute);
      break;
    case 6:
      sprintf(magrib, "%02d:%02d", shour, sminute);
      break;
    case 7:
      sprintf(isya, "%02d:%02d", shour, sminute);
      break;
    default:
      break;
        }
      }     
  
  
  }

//Julian Date at GMT mid day
float E_Julian_date(int Year,int Month,int Days,float Longitude) // juliant date - 2451545
  {

    if (Month <= 2)
      {
      Year -= 1;
      Month += 12;
      }

    float A = floor(((float)Year/100.0));

    float B = 2 - A + floor(A/4.0);

    float CLong = Longitude/(float)(15 * 24);
    float JD = floor(365.25 *(float)(Year+ 4716)) 
              - 2451545 
              + floor(30.6001 * (float)(Month + 1)) 
              + (float)Days + B 
              - 1524.5
              -CLong;
    return JD; 
  }

//Sun Declination 
float EqT(const float EJD) 
  {
    float g = fix_angle(357.529f + 0.98560028f* EJD);
    float q = fix_angle(280.459f + 0.98564736f* EJD);
    float L = fix_angle(q + 1.915* sin(d2r(g)) + 0.020* sin(d2r(2*g)));
    float e = (23.439f - 0.00000036f* EJD);

    float RA = r2d(atan2(cos(d2r(e))* sin(d2r(L)), cos(d2r(L))))/ 15;
    float Eq =(q/15-fix_hour(RA));
    return Eq;
 //   Ds = r2d(asin(sin(d2r(e))* sin(d2r(L))));  // declination of the Sun
  }
  
float Dql(float EJD) 
  {
    float g = fix_angle((357.529f + 0.98560028f* EJD));
    float q = fix_angle((280.459f + 0.98564736f* EJD));
    float L = fix_angle((q + 1.915f* sin(d2r(g)) + 0.020f* sin(d2r(2*g))));
    float e = (23.439f - 0.00000036f* EJD);

    float dd = r2d(asin(sin(d2r(e))* sin(d2r(L))));  // declination of the Sun
    return dd;
  }

float HourAngle( float Alfa, float Declination, float Latitude)
  {
     float rn =acos(
                      (-sin(d2r(Alfa))-sin(d2r(Latitude))*sin(d2r(Declination)))
                      /
                      (cos(d2r(Latitude))*cos(d2r(Declination)))
                     )/15;
     return r2d(rn);
  }

void Pray_Time(float TimeZone, float Latitude, float Longitude,float Altitude,float Declination, float EquationOfTime )
  { 
   
    float BaseTime = fix_hour((float)12+TimeZone-(Longitude/15)-EquationOfTime);
    sholatT[4] = BaseTime + (float)Prm.ihyati/60 + (float)Prm.k4/60;

  

    // Ashr
    float alfa =r2d(-atan(1 / (1+tan(d2r(fabs(Latitude-Declination))))));
    float HA = HourAngle(alfa,Declination,Latitude);
    sholatT[5] = BaseTime + HA + (float)Prm.ihyati/60+ (float)Prm.k5/60;

    // Maghrib
    alfa = 0.8333f+0.0347f*sqrt(Altitude);
    HA = HourAngle(alfa,Declination,Latitude);
    sholatT[6] = BaseTime + HA + (float)Prm.ihyati/60+ (float)Prm.k6/60;

    // Terbit
    sholatT[2] = BaseTime - HA;

    // Isya
    HA = HourAngle((float)18,Declination,Latitude);
    sholatT[7] = BaseTime + HA + (float)Prm.ihyati/60+ (float)Prm.k7/60;

    // Shubuh
    HA = HourAngle((float)20,Declination,Latitude);
    sholatT[1] = BaseTime - HA + (float)Prm.ihyati/60+ (float)Prm.k1/60;
    // Imsak
    sholatT[0] = sholatT[1]-(float)10/60;  
    // Dhuha
    HA = HourAngle((float)-4.5,Declination,Latitude);
    sholatT[3] = BaseTime - HA;
    
  char buff[100];
  }

float fix_hour(float a)
  {
    a = a - (float)24.0 * floor(a / 24.0);
    a = a < 0.0 ? a + 24.0 : a;
    return a;
  }

float fix_angle(float a)
  {
    a = a - (float)360.0 * floor(a / 360.0);
    a = a < 0.0 ? a + 360.0 : a;
    return a;
  }  


//////hijiriyah voidku/////////////////////////////////////////////////
void islam() {
  //
  waktuMagrib = {18, 12};
  RtcDateTime now = Rtc.GetDateTime();

  tanggalMasehi.tanggal = now.Day();
  tanggalMasehi.bulan = now.Month();
  tanggalMasehi.tahun = now.Year() - 2000;

  uint32_t jumlahHari;
  double jumlahHariMatahari;

  tanggalMasehi.hari = hariDariTanggal(tanggalMasehi);
  masehiKeHijriah(tanggalMasehi, waktuMagrib, KH,
                  tanggalHijriah, tanggalJawa);
  printKonversi(tanggalMasehi, tanggalHijriah, tanggalJawa);

  //while (1);
}



double get_julian_date(TanggalE tanggal)
{
  if (tanggal.bulan <= 2)
  {
    tanggal.tahun -= 1;
    tanggal.bulan += 12;
  }

  double a = floor(tanggal.tahun / 100.0);
  double b = 2 - a + floor(a / 4.0);

  if (tanggal.tahun < 1583)
    b = 0;
  if (tanggal.tahun == 1582) {
    if (tanggal.bulan > 10)
      b = -10;
    if (tanggal.bulan == 10) {
      b = 0;
      if (tanggal.tanggal > 4)
        b = -10;
    }
  }

  return floor(365.25 * (tanggal.tahun + 4716)) + floor(30.6001 *
         (tanggal.bulan + 1)) + tanggal.tanggal + b - 1524.5;
}

double konversiTanggalHijriahKeJulianDate(uint16_t tahun, uint8_t
    bulan, uint8_t tanggal)
{
  return (EH + tanggal + ceil(29.5 * (bulan - 1)) +
          (354L * (tahun - 1)) + floor((3 + (11 * tahun)) / 30)) - 1;
}

void masehiKeHijriah(TanggalDanWaktu masehi, JamDanMenit
                     waktuSholatMagrib, int8_t koreksiHijriah, TanggalE & hijriah,
                     TanggalJawa & jawa)
{
  uint16_t sisaHari;
  double julianDate = get_julian_date({masehi.tanggal,
                                       masehi.bulan, masehi.tahun + 2000
                                      });

  uint16_t menitMagrib = waktuSholatMagrib.jam * 60  +
                         waktuSholatMagrib.menit;
  uint16_t menitSekarang = masehi.jam * 60 + masehi.menit;

  if (menitSekarang >= menitMagrib)
  {
    julianDate++;//Pergantian hari hijrah pada magrib
  }

  julianDate = floor(julianDate) + 0.5;

  TanggalE tanggalHijriah;

  hijriah.tahun = floor(((30 * (julianDate - EH)) +
                         10646) / 10631);
  hijriah.bulan = min(12.0, ceil((julianDate - (29 +
                                  konversiTanggalHijriahKeJulianDate(hijriah.tahun, 1, 1))) / 29.5)
                      + 1);
  hijriah.tanggal = (julianDate -
                     konversiTanggalHijriahKeJulianDate(hijriah.tahun, hijriah.bulan,
                         1)) + 1+Prm.ch;

  long julianLong = (long)julianDate;
  jawa.pasaran = ((julianLong + 1) % 5); //0 = legi
  jawa.wuku = (((julianLong + 65) % 210) / 7); //0 = Shinta
  jawa.tanggal = hijriah.tanggal;
  jawa.bulan = hijriah.bulan;
  jawa.tahun = hijriah.tahun + 512;

}
uint8_t hariDariTanggal(TanggalDanWaktu tanggalDanWaktu) {
  uint16_t jumlahHari = tanggalDanWaktu.tanggal;
  for (uint8_t i = 1; i < tanggalDanWaktu.bulan; ++i)
    jumlahHari += jumlahHariPerBulan[i - 1];
  if (tanggalDanWaktu.bulan > 2 && tanggalDanWaktu.tahun % 4 == 0)
    ++jumlahHari;
  jumlahHari += (365 * tanggalDanWaktu.tahun) +
                ((tanggalDanWaktu.tahun + 3) / 4) - 1;

  return ((jumlahHari + 6) % 7) + 1; // 1 Januari 2000 hari sabtu= 7

}
void printKonversi(TanggalDanWaktu tanggalMasehi, TanggalE
                   tanggalHijriah, TanggalJawa tanggalJawa)
{



  ///end hijir voidku///////////////////////////////////////////////////
}
