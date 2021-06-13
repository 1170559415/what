#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DS1302.h>
#include <ESP8266WiFi.h>
#include "ESPDateTime.h"
#include <FS.h>
#include <U8g2lib.h>

/***********************************************引脚定义*******************************************************/

//-----------------------------------显示定义----------------------------------
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D4, /* data=*/ D3, /* reset=*/ U8X8_PIN_NONE);
//-----------------------------------时钟定义----------------------------------
DS1302 rtc(D6, D7, D8);
//-----------------------------------按键定义----------------------------------
//按键
#define Button_A D5
#define Button_B D2
#define Button_C D1
//检测按键
bool A = false;
bool B = false;
bool C = false;
bool Close_Button = true;
bool Open_Button = false;
/***********************************************WiFi定义******************************************************/
//连接设置
#define WIFI_SSID "xwbtd"
#define WIFI_PASS "a12345678b."
//其他设置
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiServer server(80);
const char* host = "updata";
/**********************************************时间定义*********************************************************/
//睡眠时间
unsigned long sleep_time = 1000000;
unsigned long sleep1 = 0;
unsigned long sleep2 = 0;

//以下时间使用
String Times;
String Date;
String week_day;
/**********************************************界面函数定义******************************************************/
//列表
String list[200];
//界面切换
int Interface = 0;
int Serial_number = 0;
//以下为界面模式
bool menu_open = false;//菜单
bool main_Interface_open = true;//主界面
bool ota_open = false;
bool Word_update_open = false;
//菜单界面下的小手指
unsigned char arrow[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFA, 0x3E, 0x86, 0x3F, 0x42, 0x03,
                         0xBA, 0x02, 0xE2, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        };
/*************************************************以下是按键中断程序*************************************************/
void  ICACHE_RAM_ATTR Check_A();
void Check_A() {
  A = true;
}

void  ICACHE_RAM_ATTR Check_B();
void Check_B() {
  B = true;
}

void  ICACHE_RAM_ATTR Check_C();
void Check_C() {
  C = true;
}

void Check_Button() {
  if (Close_Button) {
    A = false;
    B = false;
    C = false;
    detachInterrupt(digitalPinToInterrupt(Button_A));
    detachInterrupt(digitalPinToInterrupt(Button_B));
    detachInterrupt(digitalPinToInterrupt(Button_C));

    sleep1 = millis() + sleep_time;
    Open_Button = true;
    Close_Button = false;
  }
  if (Open_Button && sleep2 + sleep_time - 100 >= sleep1) {
    attachInterrupt(digitalPinToInterrupt(Button_A), Check_A, FALLING);
    attachInterrupt(digitalPinToInterrupt(Button_B), Check_B, FALLING);
    attachInterrupt(digitalPinToInterrupt(Button_C), Check_C, FALLING);
    Open_Button = false;
  }
}
/*************************************************显示程序*************************************************/
//居中显示
void Center_print(String string, int y) {
  u8g2.setFont(u8g2_font_ncenB12_tr); //设置字体
  u8g2.setCursor((128 - string.length() * 12) / 2 , y); //设置光标处
  u8g2.print(string);
}
//以下是汉字显示程序 (文件 , x , y, 是否换行)
void display_str(String string, int display_x , int display_y , bool line_feed) {
  int x = 0;
  int y = 0;
  int horizontal = 0;
  int vertical = 0;
  int display_bit = 0;
  String fs_name;
  String Chinese;

  File Fs_open;

  if (string.indexOf(':') != -1) {
    x = string.substring(string.indexOf(':') + 1, string.indexOf('.')).toInt();
    y = string.substring(string.indexOf('.') + 1, string.indexOf('$')).toInt();
    fs_name = "/";
    fs_name = fs_name + x + ".c";
    Fs_open = SPIFFS.open(fs_name, "r");
    string = Fs_open.readString();
    Fs_open.close();
    for (y ; y > 0; y--) {
      string = string.substring(string.indexOf('#') + 1);
    }
    string = string.substring(0, string.indexOf('#'));
  }
  else {
    Fs_open = SPIFFS.open(string, "r");
    string = Fs_open.readString();
    Fs_open.close();
  }

  while (string.indexOf('$') != -1) {
    x = string.substring(0, string.indexOf('^')).toInt();
    y = string.substring(string.indexOf('^') + 1, string.indexOf('$')).toInt();
    string = string.substring(string.indexOf('$') + 1);
    fs_name = "/";
    fs_name = fs_name + x + ".ch";
    Fs_open = SPIFFS.open(fs_name, "r");
    Chinese = Fs_open.readString();
    Fs_open.close();
    for (y ; y > 0; y--) {
      Chinese = Chinese.substring(Chinese.indexOf('$') + 1);
    }
    Chinese = Chinese.substring(0, Chinese.indexOf('$'));
    unsigned char display_char[24];
    for (int i = 0; i < 24; i++) {
      display_char[i] = Chinese.substring(0, Chinese.indexOf(',')).toInt();
      Chinese = Chinese.substring(Chinese.indexOf(',') + 1);
    }
    if (line_feed) {
      u8g2.drawXBMP(display_x + horizontal * 12 , display_y + vertical * 13 , 12, 12 , display_char);
    }
    else {
      u8g2.drawXBMP(display_x + horizontal * 12 , display_y, 12, 12 , display_char);
    }
    horizontal++;
    if (display_x + (horizontal + 1) * 12 > 128) {
      horizontal = 0;
      vertical++;
    }
  }
}

/*************************************************以下读取列表*************************************************/
void List(String string_ , String _string) {
  memset(list, '\0', sizeof(list));
  File _list;
  _list = SPIFFS.open(string_, "r");
  String string = _list.readString();
  _list.close();
  int i = 0;
  while (string.indexOf('/') != -1) {
    list[i] = string.substring(string.indexOf('/'), string.indexOf('.') + _string.length());
    string = string.substring(string.indexOf('.') + _string.length() + 1);
    i++;
  }
}
/*************************************************时钟读取程序*************************************************/
void TIME() {
  Times = rtc.getTimeStr(FORMAT_LONG);
  Date = rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN);
  week_day = "/";
  week_day = week_day + rtc.getDOWStr() + ".day";
}
/*************************************************显示界面*************************************************/
//菜单界面
void menu() {
  if (menu_open) {
    u8g2.clearBuffer();
    u8g2.drawXBMP( 20 , -1 , 16 , 16 , arrow );
    display_str(list[Interface], 40, 0, false);
    display_str(list[Interface + 1], 40, 13,  false);
    display_str(list[Interface + 2], 40, 26,  false);
    display_str(list[Interface + 3], 40, 39,  false);
    u8g2.sendBuffer();
    if (A && !Close_Button) {
      Interface--;
      Close_Button = true ;

    }
    if (C && !Close_Button) {
      Interface++;
      Close_Button = true;
    }
    if (B && !Close_Button) {
      Close_Button = true;
      switch (Interface) {
        case 0: {
            menu_open = false;
            main_Interface_open = true;
            Interface = 0;
          }
          break;
        case 3: {
            File Fs_open;
            Fs_open = SPIFFS.open("/sync_time.setup", "w+");
            Fs_open.print("1");
            Fs_open.close();
            ESP.restart();
          }
          break;
        case 4: {
            File Fs_open;
            Fs_open = SPIFFS.open("/ota.setup", "w+");
            Fs_open.print("1");
            Fs_open.close();
            ESP.restart();
          }
          break;
        case 5: {
            File Fs_open;
            Fs_open = SPIFFS.open("/word_update.setup", "w+");
            Fs_open.print("1");
            Fs_open.close();
            ESP.restart();
          }
          break;
      }
    }
  }
}
//主界面
void main_Interface() {
  if (main_Interface_open) {
    TIME();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso24_tr);
    u8g2.setCursor(7 , 43); //设置光标处
    u8g2.print(Times);
    u8g2.setFont(u8g2_font_freedoomr10_mu); //设置字体
    u8g2.setCursor(0 , 64); //设置光标处
    u8g2.print(Date);
    display_str("/fighting.day", 0, 0, false);
    File Fs_open;
    Fs_open = SPIFFS.open("/ceshi.cs" , "r");
    String cs = Fs_open.readString();
    Serial.println(cs);
    Fs_open.close();
    u8g2.setFont(u8g2_font_freedoomr10_mu); //设置字体
    u8g2.setCursor(90 , 64); //设置光标处
    u8g2.print(cs);
    u8g2.sendBuffer();
    if (B && !Close_Button) {
      menu_open = true;
      main_Interface_open = false;
      List("/menu.list", ".set");
      Close_Button = true;
    }
  }
}
//固件更新界面
void ota() {
  if (ota_open) {
    File Fs_open;
    Fs_open = SPIFFS.open("/ceshi.cs" , "w");
    Fs_open.print(analogRead(A0));
    Fs_open.close();
  }
}
//单词更新界面
void Word_update() {
  String Word_name;
  if (Word_update_open) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_timB10_tf);
    //    display_str("/update.wifi", 16, 0, false);
    u8g2.setCursor(10, 30); //设置光标处
    u8g2.print(WiFi.localIP().toString());
    u8g2.sendBuffer();
    WiFiClient client = server.available();
    if (client) {
      String req = client.readStringUntil('\r');
      String Word_write = "";
      client.flush();
      if (req.indexOf("/Fs=") != -1) {
        Word_name = "/";
        Word_name = Word_name + req.substring(req.indexOf('=') + 1, req.indexOf('$'));
        req = req.substring(req.indexOf('$') + 1);
        while (req.indexOf('$') != -1) {
          String Word_data = req.substring(0, req.indexOf('$'));
          req = req.substring(req.indexOf('$') + 1);
          Word_data += "\r\n";
          Word_write += Word_data;
        }
        File Fs_open;
        Fs_open = SPIFFS.open(Word_name , "w");
        Fs_open.print(Word_write);
        Fs_open.close();
        Fs_open = SPIFFS.open(Word_name , "r");
        client.print(Word_name);
        Fs_open.close();
        u8g2.clearBuffer();
        display_str(Word_name, 16, 0, false);
        u8g2.sendBuffer();
        delay(1000);
      }
    }
  }
}
/*************************************************初始化程序*************************************************/
//初始化WiFi
void WiFi_setup() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    u8g2.clearBuffer();
    Center_print("WiFi", 16);
    display_str("/connecting.wifi", 40, 29,  false);
    u8g2.sendBuffer();
    delay(200);
  }
  u8g2.clearBuffer();
  Center_print("WiFi", 16);
  display_str("/connected.wifi", 46, 29, false);
  u8g2.sendBuffer();
  delay(500);
}

//初始化时钟
void DateTime_setup() {
  DateTime.setTimeZone(8);
  DateTime.begin();
  DateTimeParts p = DateTime.getParts();
  rtc.setDOW(p.getWeekDay());
  rtc.setTime(p.getHours(), p.getMinutes(), p.getSeconds());
  rtc.setDate(p.getMonthDay(), p.getMonth() + 1, p.getYear());
}
// 初始化固件更新
void ota_setup() {
  httpUpdater.setup(&httpServer);
  httpServer.begin();
}
//初始化单词更新
void Word_update_setup() {
  server.begin();
}
//初始化主程序
void setup() {
  pinMode(Button_A, INPUT_PULLUP);
  pinMode(Button_B, INPUT_PULLUP);
  pinMode(Button_C, INPUT_PULLUP);
  rtc.halt(false);
  rtc.writeProtect(false);
  SPIFFS.begin();
  u8g2.begin();
  Serial.begin(9600);

  File Fs_open;
  int check;
  //----------------------检查是否同步时间
  Fs_open = SPIFFS.open("/sync_time.setup", "r+");
  check = Fs_open.readString().toInt();
  Fs_open.close();
  if (check == 1) {
    WiFi_setup();
    DateTime_setup();
    Fs_open = SPIFFS.open("/sync_time.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
    Fs_open = SPIFFS.open("/power.setup", "w+");
    Fs_open.print("1");
    Fs_open.close();
    ESP.restart();
  }
  //----------------------检查是否更新固件
  Fs_open = SPIFFS.open("/ota.setup", "r+");
  check = Fs_open.readString().toInt();
  Fs_open.close();
  if (check == 1) {
    //    WiFi_setup();
    //    ota_setup();
    Fs_open = SPIFFS.open("/ota.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
    //    Fs_open = SPIFFS.open("/sync_time.setup", "w+");
    //    Fs_open.print("1");
    //    Fs_open.close();
    //    main_Interface_open = false;
    ota_open = true;
    Fs_open = SPIFFS.open("/power.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
  }
  //----------------------检查是否更新单词
  Fs_open = SPIFFS.open("/word_update.setup", "r+");
  check = Fs_open.readString().toInt();
  Fs_open.close();
  if (check == 1) {
    WiFi_setup();
    Word_update_setup();
    Fs_open = SPIFFS.open("/word_update.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
    Fs_open = SPIFFS.open("/sync_time.setup", "w+");
    Fs_open.print("1");
    Fs_open.close();
    main_Interface_open = false;
    Word_update_open = true;
    Fs_open = SPIFFS.open("/power.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
  }
  //----------------------检查是否重启
  Fs_open = SPIFFS.open("/power.setup", "r+");
  check = Fs_open.readString().toInt();
  Fs_open.close();
  if (check == 1) {
    Fs_open = SPIFFS.open("/power.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
    ESP.deepSleep(100, WAKE_RF_DISABLED);
  }
  else  {
    Fs_open = SPIFFS.open("/power.setup", "w+");
    Fs_open.print("1");
    Fs_open.close();
  }
}

void loop() {
  Check_Button();
  ota();
  Word_update();
  if (sleep1 >= sleep2) {
    u8g2.setPowerSave(0);
    sleep2 = millis();
    menu();
    main_Interface();
    if (sleep2 >= sleep1 && !ota_open && !Word_update_open) {
      u8g2.setPowerSave(1);
    }
  }
  if (A || B || C) {
    Close_Button = true;
  }
}
