//The source is created by Gins-X. Using for business is forbidden.

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <dht11.h>
#include <EEPROM.h>

#define OLED_RESET 4
#define DHT11PIN 2
#define BUTTON1 3
#define BUTTON2 4
#define BUTTON3 5
#define BUTTON4 6
#define ALARM 7
Adafruit_SSD1306 display(OLED_RESET);
dht11 DHT11;

#define LOGO16_GLCD_HEIGHT 16 //定义显示高度
#define LOGO16_GLCD_WIDTH  16 //定义显示宽度

int clock_hour=0,clock_minute=0,clock_second=0;
double Clock,Clock_Plus,now,millis_temp;
double Button1Now,Button2Now,Button3Now,Button4Now;
int button1mode=0,button2mode=0,button3mode=0,button4mode=0;
int Cursor=0;
int menu=0;
double Alarm_Time;
int Alarm_State=0;
int Alarm_Set=0;
int Year=2022,Month=5,Day=2,Week=1;
int Day_Plus=0,Last_Day_Plus=0;
int count;
int Bluetooth_State=0;
int timeplace=0;
 
void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  pinMode(BUTTON1,INPUT);
  pinMode(BUTTON2,INPUT);
  pinMode(BUTTON3,INPUT);
  pinMode(BUTTON4,INPUT);
  pinMode(ALARM,OUTPUT);
  pinMode(13,OUTPUT);
  //Clock_Plus=9*3600+10*60+40;
  display.display();
  display.clearDisplay();
  display.setTextSize(2);             //设置字体大小
  display.setTextColor(WHITE);        //设置字体颜色白色
  display.setCursor(16,24);
  display.print("GINCLOCK");
  display.display();
  delay(1500);
  display.clearDisplay();
}

void loop() 
{
  Bluetooth();
  if(Alarm_Set==1)
  {
      Alarm_Check();
  }
  else
  {
    digitalWrite(ALARM,HIGH);
  }
  if(Button3_Scan()==1)
  {
      menu=1;
  }
  if(Button4_Scan()==1)
  {
      Alarm_Set=!Alarm_Set;
  }
  if(menu==0)
  {
      Main_Display();
  }
  else if(menu==1)
  {
      Draw_Choose(1);
      Change_Display(clock_hour,clock_minute,clock_second,millis_temp);
  }
  else if(menu==2)
  {
      Draw_Choose(2);
      Alarm_Display();
  }
  else if(menu==3)
  {
      Draw_Choose(3);
      Tamato();
  }
  else if(menu==4)
  {
      Draw_Choose(4);
      Info();
  }
  else if(menu==5)
  {
      Draw_Choose(5);
      Date();
  }
}

void Main_Display()
{
  int check=DHT11.read(DHT11PIN);
  int tem=(float)DHT11.temperature;
  int hum=(float)DHT11.humidity;
  display.clearDisplay();
  Clock=millis()/1000+Clock_Plus;
  clock_hour=(unsigned long)Clock/3600UL;
  clock_minute=(unsigned long)(Clock-3600UL*clock_hour)/60UL;
  clock_second=(unsigned long)Clock%60UL;
  Day_Plus=clock_hour/24;
  clock_hour%=24;
  if(Last_Day_Plus!=Day_Plus)
  {
    Day_Plus1();
    Last_Day_Plus=Day_Plus;
  }
  
  display.setTextSize(2);             //设置字体大小
  display.setTextColor(WHITE);        //设置字体颜色白色
  display.setCursor(16,0);             //设置字体的起始位置  
  if(clock_hour<10)
  {
    display.print("0"); 
  }
  display.print((int)clock_hour);          //输出数字并换行
  display.print(":");  
  if(clock_minute<10)
  {
    display.print("0"); 
  }
  display.print((int)clock_minute);
  display.print(":");  
  if(clock_second<10)
  {
    display.print("0"); 
  }
  display.print((int)clock_second);
  display.setTextSize(1);    

  display.setCursor(0,40);
  display.print("ALARM:"); 
  if(Alarm_Set==0)
  {
    display.print("OFF"); 
  }
  else if(Alarm_Set==1)
  {
    display.print("ON"); 
  }
  display.setCursor(0,50);  
  display.print("TEM:"); 
  display.print(tem); 
  display.print("`C");
  display.print("     HUM:"); 
  display.print(hum); 
  display.print("%");

  display.setCursor(16+3,16+8);
  display.setTextSize(1); 
  display.print(Year);
  display.print("-");
  if(Month<10)
  {
    display.print("0");
  }
  display.print(Month);
  display.print("-");
  if(Day<10)
  {
    display.print("0");
  }
  display.print(Day);
  display.print("  ");
  switch(Week)
  {
    case 1:
      display.print("MON");
    break;
    case 2:
      display.print("TUE");
    break;
    case 3:
      display.print("WED");
    break;
    case 4:
      display.print("THU");
    break;
    case 5:
      display.print("FRI");
    break;
    case 6:
      display.print("SAT");
    break;
    case 7:
      display.print("SUN");
    break;
  }
  display.display(); 
}

//SETTING
void Change_Display(long Hour,long Minute,long Second,unsigned long millis_temp)
{
  while(1)
  {
    if(Button4_Scan()==1)
    {
      if(Cursor==5)
      {
        Cursor=1;
      }
      else
      {
        Cursor++;
      }
    }

    if(Button1_Scan()==1)
    {
      if(Cursor==1)
      {
        Hour--;
      }
      if(Cursor==2)
      {
        Minute--;
      }
      if(Cursor==3)
      {
        Second--;
      }
    }

    if(Button2_Scan()==1)
    {
      if(Cursor==1)
      {
        Hour++;
      }
      if(Cursor==2)
      {
        Minute++;
      }
      if(Cursor==3)
      {
        Second++;
      }
    }
    if(Button3_Scan()==1)
    {
      delay(500);
      Cursor=0;
      menu++;
      break;
    }
    if(Cursor==4)
    {
      Clock_Plus=Hour*3600+Minute*60+Second-millis()/1000;
      if(Clock_Plus>=24UL*60UL*60UL)
      {
        Clock_Plus-=(Clock_Plus/(24UL*60UL*60UL))*Clock_Plus;
      }
      delay(500);
      Cursor=0;
      menu=0;
      break;
    }
    if(Hour==24)
    {
      Hour=0;
    }
    if(Hour==-1)
    {
      Hour=23;
    }
    if(Minute==60)
    {
      Minute=0;
    }
    if(Minute==-1)
    {
      Minute=59;
    }
    if(Second==60)
    {
      Second=0;
    }
    if(Second==-1)
    {
      Second=59;
    }
    
    display.clearDisplay();
    Clock=millis()/1000+Clock_Plus;
    clock_hour=(unsigned long)Clock/3600;
    clock_minute=(unsigned long)(Clock-3600*clock_hour)/60;
    clock_second=(unsigned long)Clock%60;

    display.setTextSize(1);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(43,0);       //设置字体的起始位置  
    display.print("SETTING");

    display.setTextSize(2);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(16,25);       //设置字体的起始位置  
    if(Cursor==1)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Hour<10)
    {
      display.print("0");
    }
    display.print(Hour);          //输出数字并换行
    display.setTextColor(WHITE,BLACK);
    display.print(":");  
    if(Cursor==2)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Minute<10)
    {
      display.print("0");
    }
    display.print(Minute);
    display.setTextColor(WHITE,BLACK);
    display.print(":");  
    if(Cursor==3)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Second<10)
    {
      display.print("0");
    }
    display.print(Second);  
    display.display(); 
  }
}

void Alarm_Display()
{
  long Hour,Minute,Second;
  Hour=(long)EEPROM.read(0);
  Minute=(long)EEPROM.read(1);
  Second=(long)EEPROM.read(2);
  while(1)
  {
    if(Button4_Scan()==1)
    {
      if(Cursor==5)
      {
        Cursor=1;
      }
      else
      {
        Cursor++;
      }
    }

    if(Button1_Scan()==1)
    {
      if(Cursor==1)
      {
        Hour--;
      }
      if(Cursor==2)
      {
        Minute--;
      }
      if(Cursor==3)
      {
        Second--;
      }
    }

    if(Button2_Scan()==1)
    {
      if(Cursor==1)
      {
        Hour++;
      }
      if(Cursor==2)
      {
        Minute++;
      }
      if(Cursor==3)
      {
        Second++;
      }
    }
    if(Button3_Scan()==1)
    {
      delay(500);
      Cursor=0;
      menu++;
      break;
    }
    if(Cursor==4)
    {
      EEPROM.write(0,(char)Hour);
      EEPROM.write(1,(char)Minute);
      EEPROM.write(2,(char)Second);
      delay(500);
      Cursor=0;
      menu=0;
      break;
    }
    if(Hour==24)
    {
      Hour=0;
    }
    if(Hour==-1)
    {
      Hour=23;
    }
    if(Minute==60)
    {
      Minute=0;
    }
    if(Minute==-1)
    {
      Minute=59;
    }
    if(Second==60)
    {
      Second=0;
    }
    if(Second==-1)
    {
      Second=59;
    }
    
    display.clearDisplay();
    Clock=millis()/1000+Clock_Plus;
    clock_hour=(unsigned long)Clock/3600;
    clock_minute=(unsigned long)(Clock-3600*clock_hour)/60;
    clock_second=(unsigned long)Clock%60;

    display.setTextSize(1);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(49,0);       //设置字体的起始位置  
    display.print("ALARM");


    
    display.setTextSize(2);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(16,25);       //设置字体的起始位置  
    if(Cursor==1)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Hour<10)
    {
      display.print("0");
    }
    display.print(Hour);          //输出数字并换行
    display.setTextColor(WHITE,BLACK);
    display.print(":");  
    if(Cursor==2)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Minute<10)
    {
      display.print("0");
    }
    display.print(Minute);
    display.setTextColor(WHITE,BLACK);
    display.print(":");  
    if(Cursor==3)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Second<10)
    {
      display.print("0");
    }
    display.print(Second);  
    display.display(); 
  }
}

void Tamato()
{
  int Learning_Minute=25,Learning_Second=0,Resting_Minute=5,Resting_Second=0;
  double Start_Temp;
  int State=0;
  while(1)
  {
    if(Button1_Scan()==1)
    {
      State++;
    }
    if(State==0)
    {
      Learning_Minute=25;
      Learning_Second=0;
      Tamato_Display(Learning_Minute,Learning_Second,State);
    }
    
    if(State==1)
    {
      Start_Temp=millis();
      while(1)
      {
        if((Start_Temp-millis())/1000UL==25UL*60UL)
        {
          State=2;
          EEPROM.write(5,EEPROM.read(5)+1);
          break;
        }

        
        Learning_Second=60-(int)((millis()-Start_Temp)/1000UL)%60;
        Learning_Minute=(int)((25UL*1000UL*60UL-(millis()-Start_Temp))/1000UL/60UL);
        if(Learning_Second==60)
        {
          Learning_Second=0;
        }
        if(Learning_Second==0)
        {
          Learning_Minute+=1;
        }
        if(Learning_Minute==26)
        {
          Learning_Minute=25;
        }
        
        Tamato_Display(Learning_Minute,Learning_Second,State);
        if(Button2_Scan()==1)
        {
          State=0;
          Learning_Minute=25;
          Learning_Second=0;
          break;
        }
      }
    }
    if(State==2)
    {
      Start_Temp=millis();
      while(1)
      {
        if((Start_Temp-millis())/1000UL==5UL*60UL)
        {
          State=1;
          break;
        }

        
        Resting_Second=60-(int)((millis()-Start_Temp)/1000UL)%60;
        Resting_Minute=(int)((5UL*1000UL*60UL-millis()-Start_Temp)/1000UL/60UL);
        if(Resting_Second==60)
        {
          Resting_Second=0;
        }
        if(Resting_Second==0)
        {
          Resting_Minute+=1;
        }
        
        Tamato_Display(Resting_Minute,Resting_Second,State);
        if(Button2_Scan()==1)
        {
          State=0;
          Learning_Minute=25;
          Learning_Second=0;
          break;
        }
      }
    }

    if(Button3_Scan()==1)
    {
      menu++;
      break;
    }
    
  }
}

void Tamato_Display(int Minute,int Second,int State)
{
    display.clearDisplay();
    
    display.setTextSize(1);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(46,0);       //设置字体的起始位置  
    display.print("Tamato");


    display.setTextSize(2);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    if(State==1&&((int)millis()/666)%2==0)
    {
      display.setCursor(0,48); 
      display.print("Learning..");
    }
    if(State==2&&((int)millis()/666)%2==0)
    {
      display.setCursor(0,48); 
      display.print("Resting...");
    }
    display.setCursor(16+18,25);       //设置字体的起始位置  
    if(Minute<10)
    {
      display.print("0");
    }
    display.print(Minute);
    display.setTextColor(WHITE,BLACK);
    display.print(":");  
    if(Second<10)
    {
      display.print("0");
    }
    display.print(Second);  
    display.display(); 
}

void Info()
{
  while(1)
  {

      display.clearDisplay();
          
      display.setTextSize(1);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(34,0);       //设置字体的起始位置  
      display.print("Infomation");


      display.setTextSize(2);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(0,16);
      display.print("Early:");
      display.print(EEPROM.read(3));
      display.setCursor(0,32);
      display.print("Lazy:");
      display.print(EEPROM.read(4));
      display.setCursor(0,48);
      display.print("Tamato:");
      display.print(EEPROM.read(5));
      display.display();

      if(Button3_Scan()==1)
      {
        menu++;
        break;
      }
  }
}

void Date()
{
  while(1)
  {
    if(Button4_Scan()==1)
    {
      if(Cursor==6)
      {
        Cursor=1;
      }
      else
      {
        Cursor++;
      }
    }

    if(Button1_Scan()==1)
    {
      if(Cursor==1)
      {
        Year--;
      }
      if(Cursor==2)
      {
        Month--;
      }
      if(Cursor==3)
      {
        Day--;
      }
      if(Cursor==4)
      {
        Week--;
      }
    }

    if(Button2_Scan()==1)
    {
      if(Cursor==1)
      {
        Year++;
      }
      if(Cursor==2)
      {
        Month++;
      }
      if(Cursor==3)
      {
        Day++;
      }
      if(Cursor==4)
      {
        Week++;
      }
    }
    if(Button3_Scan()==1)
    {
      delay(500);
      Cursor=0;
      menu=0;
      break;
    }
    if(Cursor==5)
    {
      delay(500);
      Cursor=0;
      menu=0;
      break;
    }
    
    if(Week==8)
    {
      Week=1;
    }
    if(Week==0)
    {
      Week=7;
    }
    
    if(Year==2051)
    {
      Year=2000;
    }
    if(Year==1999)
    {
      Year=2050;
    }
    if(Month==13)
    {
      Month=1;
    }
    if(Month==0)
    {
      Month=12;
    }

    if(Month==1||Month==3||Month==5||Month==7||Month==8||Month==10||Month==12)
    {
      if(Day==32)
      {
        Day=1;
      }
      if(Day==0)
      {
        Day=31;
      }
    }
    if(Month==4||Month==6||Month==9||Month==11)
    {
      if(Day==31)
      {
        Day=1;
      }
      if(Day==0)
      {
        Day=30;
      }
    }
    if(Month==2)
    {
      if(Year%4==0)
      {
        if(Day==30)
        {
          Day=1;
        }
        if(Day==0)
        {
          Day=29;
        }
      }
      else
      {
        if(Day==29)
        {
          Day=1;
        }
        if(Day==0)
        {
          Day=28;
        }
      }
    }

    display.clearDisplay();
    
    display.setTextSize(1);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(52,0);       //设置字体的起始位置  
    display.print("Date");


    display.setTextSize(2);             //设置字体大小
    display.setTextColor(WHITE);        //设置字体颜色白色
    display.setCursor(0,25);       //设置字体的起始位置  
    if(Cursor==1)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    display.print(Year);          //输出数字并换行
    display.setTextColor(WHITE,BLACK);
    display.print("-");  
    if(Cursor==2)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Month<10)
    {
      display.print("0");
    }
    display.print(Month);
    display.setTextColor(WHITE,BLACK);
    display.print("-");  
    if(Cursor==3)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    if(Day<10)
    {
      display.print("0");
    }
    display.print(Day);  

    display.setCursor(46,25+16);
    if(Cursor==4)
    {
      display.setTextColor(BLACK, WHITE);
    }
    else
    {
      display.setTextColor(WHITE);
    }
    switch(Week)
    {
      case 1:
        display.print("MON");
      break;
      case 2:
        display.print("TUE");
      break;
      case 3:
        display.print("WED");
      break;
      case 4:
        display.print("THU");
      break;
      case 5:
        display.print("FRI");
      break;
      case 6:
        display.print("SAT");
      break;
      case 7:
        display.print("SUN");
      break;
    }
 
    display.display(); 
  }
}


//BUTTON_SCAN
int Button1_Scan()
{
  int state=0;
  if(digitalRead(BUTTON1)==1&&button1mode==0)
  {
    Button1Now=millis();
    button1mode=1;
    //short Press
    state=1;
  }
  else if(digitalRead(BUTTON1)==0&&millis()-Button1Now>100)
  {
    button1mode=0;
    state=0;
  }
  else if(digitalRead(BUTTON1)==1&&millis()-Button1Now>1000)
  {
    state=2;
    while(digitalRead(BUTTON1))
    {
      //long Press
    }
    button1mode=0;
  }
  return state;
}

int Button2_Scan()
{
  int state=0;
  if(digitalRead(BUTTON2)==1&&button2mode==0)
  {
    Button2Now=millis();
    button2mode=1;
    //short Press
    state=1;
  }
  if(digitalRead(BUTTON2)==0&&millis()-Button2Now>100)
  {
    button2mode=0;
    state=0;
  }
    if(digitalRead(BUTTON2)==1&&millis()-Button2Now>1000)
  {
    state=2;
    /*while(digitalRead(BUTTON2))
    {
      //long Press
    }*/
    button2mode=0;
  }
  return state;
}

int Button3_Scan()
{
  int state=0;
  if(digitalRead(BUTTON3)==1&&button3mode==0)
  {
    Button3Now=millis();
    button3mode=1;
    //short Press
    state=1;
  }
  if(digitalRead(BUTTON3)==0&&millis()-Button3Now>100)
  {
    button3mode=0;
    state=0;
  }
    if(digitalRead(BUTTON3)==1&&millis()-Button3Now>1000)
  {
    state=2;
    while(digitalRead(BUTTON3))
    {
      //long Press
    }
    button3mode=0;
  }
  return state;
}

int Button4_Scan()
{
  int state=0;
  if(digitalRead(BUTTON4)==1&&button4mode==0)
  {
    Button4Now=millis();
    button4mode=1;
    //short Press
    state=1;
  }
  if(digitalRead(BUTTON4)==0&&millis()-Button4Now>100)
  {
    button4mode=0;
    state=0;
  }
    if(digitalRead(BUTTON4)==1&&millis()-Button4Now>1000)
  {
    while(digitalRead(BUTTON4))
    {
      state=2;
      //long Press
    }
    button4mode=0;
  }
  return state;
}


//MENU
void Draw_Choose(int menu)
{
  switch(menu)
  {
    case 1:
      display.clearDisplay();
      display.setTextSize(3);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(1,20);
      display.print("SETTING");
      display.display();
      delay(500);
    break;
    case 2:
      display.clearDisplay();
      display.setTextSize(3);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(19,20);
      display.print("ALARM");
      display.display();
      delay(500);
    break;
    case 3:
      display.clearDisplay();
      display.setTextSize(3);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(10,20);
      display.print("TAMATO");
      display.display();
      delay(500);
    break;
    case 4:
      display.clearDisplay();
      display.setTextSize(3);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(28,20);
      display.print("INFO");
      display.display();
      delay(500);
    break;
    case 5:
      display.clearDisplay();
      display.setTextSize(3);             //设置字体大小
      display.setTextColor(WHITE);        //设置字体颜色白色
      display.setCursor(28,20);
      display.print("DATE");
      display.display();
      delay(500);
    break;
  }
}

void Alarm_Check()
{
  if((char)clock_hour==EEPROM.read(0)&&(char)clock_minute==EEPROM.read(1)&&(char)clock_second==EEPROM.read(2))
  {
    Alarm_Time=millis();
    Alarm_State=1;
  }
  if(Alarm_State==1&&millis()-Alarm_Time<=10*60*1000)
  {
    digitalWrite(ALARM,LOW);
    if(Button1_Scan()==1||Button2_Scan()==1)
    {
      EEPROM.write(3,EEPROM.read(3)+1);
      Alarm_State=0;
    }
  }
  if(Alarm_State==1&&millis()-Alarm_Time>10*60*1000)
  {
    digitalWrite(ALARM,HIGH);
    Alarm_State=0;
    EEPROM.write(4,EEPROM.read(4)+1);
  }
  if(Alarm_State==0)
  {
    digitalWrite(ALARM,HIGH);
  }
}

void Day_Plus1()
{
  Day++;
  Week++;
  
  if(Week==8)
  {
    Week=1;
  }
  if(Week==0)
  {
    Week=7;
  }
  
  if(Year==2051)
  {
    Year=2000;
  }
  if(Year==1999)
  {
    Year=2050;
  }
  if(Month==13)
  {
    Month=1;
  }
  if(Month==0)
  {
    Month=12;
  }

  if(Month==1||Month==3||Month==5||Month==7||Month==8||Month==10||Month==12)
  {
    if(Day==32)
    {
      Day=1;
    }
    if(Day==0)
    {
      Day=31;
    }
  }
  if(Month==4||Month==6||Month==9||Month==11)
  {
    if(Day==31)
    {
      Day=1;
    }
    if(Day==0)
    {
      Day=30;
    }
  }
  if(Month==2)
  {
    if(Year%4==0)
    {
      if(Day==30)
      {
        Day=1;
      }
      if(Day==0)
      {
        Day=29;
      }
    }
    else
    {
      if(Day==29)
      {
        Day=1;
      }
      if(Day==0)
      {
        Day=28;
      }
    }
  }
}

void Bluetooth()
{
  char read_temp;
  char num_temp[6]={0};
  if(Serial.available()&&Bluetooth_State==0)
  {
    Serial.print("Welcome to use GINCLOCK\nEnter:\n1 for Alarm Setting\n2 for Alarm On\n3 for Alarm Off\n4 for Achievement\n");
    while(Serial.read()>=0);
    Bluetooth_State=1;
  }
  if(Bluetooth_State==1)
  {
     
    if(timeplace==1)
    {
      if(Serial.available())
      {
        for(count=0;count<6;count++)
        {
          num_temp[count]=Serial.read()-'0';
        }
        if(num_temp[0]*10+num_temp[1]<=23&&num_temp[0]*10+num_temp[1]>=0)
        EEPROM.write(0,num_temp[0]*10+num_temp[1]);
        if(num_temp[2]*10+num_temp[3]<=59&&num_temp[2]*10+num_temp[3]>=0)
        EEPROM.write(1,num_temp[2]*10+num_temp[3]);
        if(num_temp[4]*10+num_temp[5]<=59&&num_temp[4]*10+num_temp[5]>=0)
        EEPROM.write(2,num_temp[4]*10+num_temp[5]);
        Serial.print("Success!\n");
        while(Serial.read()>=0);
        timeplace=0;
        Bluetooth_State=0;
      }
    }
    else
    {
      read_temp=Serial.read();
      
      if(read_temp=='1')
      {
        timeplace=1;
        Serial.print("Enter:\n");
        while(Serial.read()>=0);
      }
      else if(read_temp=='2')
      {
        Alarm_Set=1;
        Serial.print("Success!");
        Bluetooth_State=0;
        while(Serial.read()>=0);
      }
      else if(read_temp=='3')
      {
        Alarm_Set=0;
        Serial.print("Success!");
        Bluetooth_State=0;
        while(Serial.read()>=0);
      }
      else if(read_temp=='4')
      {
        //while(Serial.read()>=0);
        Serial.print("EarlyNum:");
        Serial.print(EEPROM.read(3));
        Serial.print("\n");
        Serial.print("LazyNum:");
        Serial.print(EEPROM.read(4));
        Serial.print("\n");
        Serial.print("TamatoNum:");
        Serial.print(EEPROM.read(5));
        Serial.print("\n");
        Bluetooth_State=0;
        while(Serial.read()>=0);
      }
    }
  }
}
