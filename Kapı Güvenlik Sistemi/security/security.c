#include <security.h>
#fuses XT,NOWDT,NOPUT,NOLVP,NOCPD,NOPROTECT,NODEBUG,NOBROWNOUT,NOWRT  

#use delay(clock=4000000)

#use fast_io(b)
#use fast_io(d)
#define use_portb_lcd TRUE

#define LCD_ENABLE_PIN PIN_B5
#define LCD_RS_PIN PIN_B7
#define LCD_RW_PIN PIN_B6
#define LCD_DATA4 PIN_B4
#define LCD_DATA5 PIN_B3
#define LCD_DATA6 PIN_B2
#define LCD_DATA7 PIN_B1

#include <lcd.c>
#define sut1 pin_d0
#define sut2 pin_d1
#define sut3 pin_d2
#define sut4 pin_d3

#define sat1 pin_d4
#define sat2 pin_d5
#define sat3 pin_d6
#define sat4 pin_d7

#define green pin_c0
#define red pin_c1

char tus=0;
int basildi;
int sifre[4];

char keypad_oku()
{
  output_b(0x00);
  tus = "";
  output_high(sat1); //1.sat�r okunuyor
  if(input(sut1)) //1.s�tun okunuyor
  {delay_ms(500); tus=1;basildi=1;}
  if(input(sut2)) //2.s�tun okunuyor
  {delay_ms(500); tus=2;basildi=1;}
  if(input(sut3)) //3.s�tun okunuyor
  {delay_ms(500); tus=3;basildi=1;}
  if(input(sut4)) //4.s�tun okunuyor
  {delay_ms(500); tus=0xA;basildi=1;}

  output_low(sat1); //1.sat�r bitti
  
  output_high(sat2); //2.sat�r okunuyor
  if(input(sut1)) //1.s�tun okunuyor
  {delay_ms(500); tus=4;basildi=1;}
  if(input(sut2)) //2.s�tun okunuyor
  {delay_ms(500); tus=5;basildi=1;}
  if(input(sut3)) //3.s�tun okunuyor
  {delay_ms(500); tus=6;basildi=1;}
  if(input(sut4)) //4.s�tun okunuyor
  {delay_ms(500); tus=0xB;basildi=1;}
  output_low(sat2); //2.sat�r bitti
  
  output_high(sat3); //3.sat�r okunuyor
  if(input(sut1)) //1.s�tun okunuyor
  {delay_ms(500); tus=7;basildi=1;}
  if(input(sut2)) //2.s�tun okunuyor
  {delay_ms(500); tus=8;basildi=1;}
  if(input(sut3)) //3.s�tun okunuyor
  {delay_ms(500); tus=9;basildi=1;}
  if(input(sut4)) //4.s�tun okunuyor
  {delay_ms(500); tus=0x0C;basildi=1;}
  output_low(sat3); //3.sat�r bitti
  
  output_high(sat4); //4.sat�r okunuyor
  if(input(sut1)) //1.s�tun okunuyor
  {delay_ms(500); tus=0xE;basildi=1;}
  if(input(sut2)) //2.s�tun okunuyor
  {delay_ms(500); tus=0;basildi=1;}
  if(input(sut3)) //3.s�tun okunuyor
  {delay_ms(500); tus=0xF;basildi=1;}
  if(input(sut4)) //4.s�tun okunuyor
  {delay_ms(500); tus=0x0;basildi=1;}
  output_low(sat4); //4.sat�r bitti
  
  return tus;

}


void main()
{
  setup_psp(PSP_DISABLED); //PSP B�R�M� DEVRE DI�I
  setup_timer_1(T1_DISABLED); //t1 zamanlay�c� devre d���
  setup_timer_2(T2_DISABLED,0,1); //t2 zamanlay�c� devre d���
  setup_adc_ports(NO_ANALOGS); //analog giri� yok
  setup_adc(ADC_Off);
  setup_CCP1(CCP_OFF);
  setup_CCP2(CCP_OFF);
  
  set_tris_b(0x00);
  set_tris_d(0b00001111);
  
  lcd_init(); //lcd haz�rlan�yor
  
  printf(lcd_putc,"\fSifre Giriniz");
  delay_ms(1500);
  
  //program�m�z�n �ifresini tan�ml�yoruz
  sifre[0] = 3;
  sifre[1] = 7;
  sifre[2] = 3;
  sifre[3] = 6;
  
  int sec=0;
  int yanlis=0;
  int gelen;
  
  while(true)
  {
    gelen = keypad_oku(); //bas�lan tu�u alg�lamas� i�in gereken fonksiyon �a��r�l�r
    if (basildi==1)
    {
      basildi=0;
      
      lcd_gotoxy(7+sec,2);
      printf(lcd_putc,"*");
      
      if(sifre[sec] != gelen)
      {
        yanlis=1;
        sec++;
      }
      if(sifre[sec] == gelen)
      {
        sec++;
      }
      
      if(sec>3)
      { 
        if(yanlis==0)
        { 
          printf(lcd_putc,"\fSifre Dogru");
          Output_high(green);
          delay_ms(2000);
          Output_low(green);
          printf(lcd_putc,"\fSifre Giriniz");
        }
        else
        {
          printf(lcd_putc,"\fSifre Yanlis");
          Output_high(red);
          delay_ms(2000);
          Output_low(red);
          lcd_gotoxy(15,2);
          delay_ms(1500);
          yanlis=0;
          printf(lcd_putc,"\fSifre Giriniz");
        }
        sec=0;
      }
    }
  }
}



