#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DS1302.h>
#include <ESP8266WiFi.h>
#include "ESPDateTime.h"
#include <FS.h>
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D2, /* data=*/ D1, /* reset=*/ U8X8_PIN_NONE);

DS1302 rtc(D5, D6, D7);

//WiFi定义
const char* host = "updata";
#define WIFI_SSID "xwbtd"
#define WIFI_PASS "a12345678b."

int sleep_time = 10000;

ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiServer server(80);


//以下时间使用
unsigned long ms = millis();
unsigned long sleep1;
unsigned long sleep2;
String Times;
String Date;
String week_day;
//int up_time;
//int seconds;
//int minutes;
//int hours;
//int day;
//int week;
//int month;
//int year;
//bool setup_time;
//String colon = ":";

String list[200];

bool A_ = true;
bool B_ = true;
bool C_ = true;

int Interface;
int Serial_number;

//以下为界面模式
bool menu_open = false;//菜单
bool main_Interface_open = true;//主界面
bool Recite_Word_open = false;//记单词
bool Recite_Word_English_open = false;
bool Recite_Word_Chinese_open = false;
bool ota_open = false;
bool Word_update_open = false;

//菜单界面下的小手指
unsigned char arrow[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFA, 0x3E, 0x86, 0x3F, 0x42, 0x03,
                         0xBA, 0x02, 0xE2, 0x03, 0xFC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        };

//以下是按键去抖
//A键
bool A() {
  if (digitalRead(D8) == LOW && A_ == true) {
    A_ = false;
    sleep1 = millis() + sleep_time;
    return true;
  }
  else if (digitalRead(D8) == HIGH && A_ == false) {
    A_ = true;
    return false;
  }
  else {
    return false;
  }
}
//B键
bool B() {
  if (digitalRead(D9) == LOW && B_ == true) {
    B_ = false;
    sleep1 = millis() + sleep_time;
    return true;
  }
  else if (digitalRead(D9) == HIGH && B_ == false) {
    B_ = true;
    return false;
  }
  else {
    return false;
  }
}
//C键
bool C() {
  if (digitalRead(D10) == LOW && C_ == true) {
    C_ = false;
    sleep1 = millis() + sleep_time;
    return true;
  }
  else if (digitalRead(D10) == HIGH && C_ == false) {
    C_ = true;
    return false;
  }
  else {
    return false;
  }
}

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

//以下读取列表
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
//格式化字符串
//String Format(int number , String string) {
//  String zero = "0";
//  if (number < 10)
//    return string + zero + number;
//  else
//    return string + number;
//}
//时间函数
//void Time() {
//  if (millis() - ms > 1000) {
//    ms = millis();
//    up_time = ms / 1000;
//    ms = up_time * 1000;
//    if (setup_time) {
//      seconds = seconds + up_time;
//      setup_time = false;
//    }
//    else {
//      seconds ++;
//    }
//    File Fs_time;
//    Fs_time = SPIFFS.open("/read.time", "w+");
//    Fs_time.print(year + colon + month + colon + day + colon + week + colon + hours + colon +  minutes + colon + seconds + colon);
//    Fs_time.close();
//    while (seconds >= 60) {
//      seconds -= 60;
//      minutes ++;
//      while (minutes >= 60) {
//        minutes -= 60;
//        hours ++;
//        while (hours >= 24) {
//          hours -= 24;
//          day ++;
//          week ++;
//          if (week == 7) {
//            week = 0;
//          }
//          if ((month == 1 || 3 || 5 || 7 || 8 || 10 || 12) && day == 32) {
//            day = 1;
//            month ++;
//          }
//          else if ((month == 4 || 6 || 9 || 11) && day == 31) {
//            day = 1;
//            month ++;
//          }
//          else if ((month == 2 && day == 30 && year % 4 == 0 && year % 100 != 0) || (month == 2 && day == 30 && year % 400 == 0)) {
//            day = 1;
//            month ++;
//          }
//          else if ((month == 2 && day == 30 && year % 4 != 0 && year % 100 == 0) || (month == 2 && day == 30 && year % 400 != 0)) {
//            day = 1;
//            month ++;
//          }
//          if (month == 13) {
//            month = 1;
//            year ++;
//          }
//        }
//      }
//    }
//    times = Format(hours , ":").substring(1) + Format(minutes , ":") + Format(seconds , ":");
//    date = Format(year , "/").substring(1) + Format(month, "/") + Format(day, "/");
//    week_day = ".day";
//    week_day = Format(week, "/week") + week_day;
//  }
//}
void TIME() {
  Times = rtc.getTimeStr(FORMAT_LONG);
  Date = rtc.getDateStr(FORMAT_LONG, FORMAT_LITTLEENDIAN);
  week_day = ".day";
  week_day = rtc.getDOWStr() + week_day;
}
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
    if (A()) {
      Interface--;
    }
    if (C()) {
      Interface++;
    }
    if (B()) {
      switch (Interface) {
        case 0: {
            menu_open = false;
            main_Interface_open = true;
            Interface = 0;
          }
          break;
        case 1: {
            menu_open = false;
            Recite_Word_open = true;
            List("/book.list", ".bak");
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
    display_str(week_day, 92, 52,  false);
    u8g2.sendBuffer();
    if (B()) {
      menu_open = true;
      main_Interface_open = false;
      List("/menu.list", ".set");
    }
  }
}

//记单词的界面
void Recite_Word() {
  if (Recite_Word_open) {
    u8g2.clearBuffer();
    u8g2.drawXBMP( 10 , -1 , 16 , 16 , arrow );
    display_str(list[Interface], 30, 0,  false);
    display_str(list[Interface + 1], 30, 13,  false);
    display_str(list[Interface + 2], 30, 26,  false);
    display_str(list[Interface + 3], 30, 39,  false);
    u8g2.sendBuffer();
    if (A()) {
      Interface--;
    }
    if (C()) {
      Interface++;
    }
    if (B()) {
      switch (Interface) {
        case 0:
          menu_open = true;
          Recite_Word_open = false;
          List("/menu.list", ".set");
          Interface = 0;
          break;
        case 13:
          menu_open = true;
          Recite_Word_open = false;
          List("/menu.list", ".set");
          Interface = 0;
          break;
        default :
          Recite_Word_open = false;
          Recite_Word_English_open = true;
          List(list[Interface].substring(list[Interface].indexOf('/'), list[Interface].indexOf('.')) + ".list", ".c");
          Serial_number = 0;
          Interface = 0;
          break;
      }
    }
  }
}


//记单词主函数
void Recite_Word_Display_English() {
  if (Recite_Word_English_open) {
    u8g2.clearBuffer();
    Center_print(list[Serial_number].substring(list[Serial_number].indexOf('/') + 1, list[Serial_number].indexOf('.')), 12);
    display_str("/jizhuli.set", 28, 27,  false);
    display_str("/ci.set", 88, 27,  false);
    display_str("/weijizhu.set", 28, 40,  false);
    display_str("/ci.set", 88, 40,  false);
    u8g2.sendBuffer();
    if (A() || C()) {
      Recite_Word_English_open = false;
      Recite_Word_Chinese_open = true;
    }
    if (B()) {
      Recite_Word_open = true;
      Recite_Word_English_open = false;
      List("/book.list", ".bak");
    }
  }
}

void Recite_Word_Display_Chinese() {
  if (Recite_Word_Chinese_open) {
    u8g2.clearBuffer();
    display_str("/jizhuli.set", 0, 51,  false);
    display_str("/weijizhu.set", 91, 51,  false);
    display_str(list[Serial_number], 0, 13,  true);
    u8g2.sendBuffer();
    if (A() || C()) {
      Recite_Word_English_open = true;
      Recite_Word_Chinese_open = false;
      Serial_number++;
    }
    if (B()) {
      Recite_Word_open = true;
      Recite_Word_Chinese_open = false;
      List("/book.list", ".bak");
    }
  }
}
//固件更新界面
void ota() {
  if (ota_open) {
    httpServer.handleClient();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_timB10_tf);
    display_str("/update.wifi", 16, 0, false);
    u8g2.setCursor(10, 30); //设置光标处
    String up_data = "/update";
    u8g2.print(WiFi.localIP().toString() + up_data);
    u8g2.sendBuffer();
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
        u8g2.clearBuffer();
        display_str(Word_name, 16, 0, false);
        u8g2.sendBuffer();
        delay(1000);
      }
    }
  }
}

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

//void DateTime_setup() {
//  File Fs_time;
//  Fs_time = SPIFFS.open("/read.time", "w+");
//  DateTime.setTimeZone(8);
//  DateTime.begin();
//  DateTimeParts p = DateTime.getParts();
//  year = p.getYear();
//  month = p.getMonth() + 1;
//  day = p.getMonthDay();
//  week = p.getWeekDay();
//  hours = p.getHours();
//  minutes = p.getMinutes();
//  seconds = p.getSeconds();
//  Fs_time.print(year + colon + month + colon + day + colon + week + colon + hours + colon +  minutes + colon + seconds + colon);
//  Fs_time.close();
//}
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
//初始化程序

void setup() {
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, LOW);
  pinMode(D8, INPUT_PULLUP);
  pinMode(D9, INPUT_PULLUP);
  pinMode(D10, INPUT_PULLUP);
  rtc.halt(false);
  rtc.writeProtect(false);
  SPIFFS.begin();
  u8g2.begin();
  TIME();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setCursor(7 , 43); //设置光标处
  u8g2.print(Times);
  u8g2.setFont(u8g2_font_freedoomr10_mu); //设置字体
  u8g2.setCursor(0 , 64); //设置光标处
  u8g2.print(Date);
  display_str("/fighting.day", 0, 0, false);
  display_str(week_day, 92, 52,  false);
  u8g2.sendBuffer();
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
  //  else {
  //    File Fs_time;
  //    Fs_time = SPIFFS.open("/read.time", "r");
  //    String string = Fs_time.readString();
  //    year = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    month = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    day = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    week = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    hours = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    minutes = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    seconds = string.substring(0, string.indexOf(':')).toInt();
  //    string = string.substring(string.indexOf(':') + 1);
  //    setup_time = true;
  //    Fs_time.close();
  //    Fs_open.close();
  //  }
  //----------------------检查是否更新固件
  Fs_open = SPIFFS.open("/ota.setup", "r+");
  check = Fs_open.readString().toInt();
  Fs_open.close();
  if (check == 1) {
    WiFi_setup();
    ota_setup();
    Fs_open = SPIFFS.open("/ota.setup", "w+");
    Fs_open.print("0");
    Fs_open.close();
    Fs_open = SPIFFS.open("/sync_time.setup", "w+");
    Fs_open.print("1");
    Fs_open.close();
    main_Interface_open = false;
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
  sleep1 = millis() + sleep_time;
}


void loop() {
  ota();
  Word_update();
  if (sleep1 >= sleep2) {
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    u8g2.setPowerSave(0);
    sleep2 = millis();
    menu();
    main_Interface();
    Recite_Word();
    Recite_Word_Display_English();
    Recite_Word_Display_Chinese();
    if (sleep2 >= sleep1 && !ota_open && !Word_update_open) {
      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH);
      u8g2.setPowerSave(1);
    }
  }
  else {
    A();
    B();
    C();
  }
}
