#include <brzo_i2c.h>
#include "SSD1306Brzo.h"

SSD1306Brzo display(0x3c, D4, D3);

#include <FS.h>

unsigned char Chinese[2][100];

void setup() {
  Serial.begin(9600);
  Serial.println();
  SPIFFS.begin();
  display.init();
  display.flipScreenVertically();
//  display.setContrast(20,255,0);

  display.setFont(Dialog_plain_12);
  display.drawString(0,0,Word(256));
//  Serial.println(Word(256));
  display_chinese(0, 13);
  display.display();
}


String Word(int open_word) {
  uint8_t book_bit[4];
  File book;
  unsigned char book_number = open_word / 200;
  int word_number = open_word % 200 - 1;
  book = SPIFFS.open("/" + (String)book_number + ".book" , "r");
  book.seek(word_number * 4 , SeekSet);
  book.read(book_bit, 4);
  int word_seek = (int)book_bit[0] * 128 + (int)book_bit[1];
  unsigned char English_long = book_bit[2];
  unsigned char Chinese_long = book_bit[3];
  Chinese[0][0] = Chinese_long;
  book.seek(word_seek, SeekSet);
  String English;
  for (English_long; English_long > 0; English_long--) {
    book.read(book_bit, 1);
    English += (char)book_bit[0];
  }
  for (Chinese_long; Chinese_long > 0; Chinese_long--) {
    book.read(book_bit, 2);
    Chinese[0][Chinese_long] = book_bit[0];
    Chinese[1][Chinese_long] = book_bit[1];
  }
  book.close();
  return English;
}

void display_chinese(int display_x, int display_y) {

  char horizontal = 0;
  char vertical = 0;
  bool font_set = false;

  for (Chinese[0][0]; Chinese[0][0] > 0; Chinese[0][0]--) {

    if (font_set) {
      display_x -= 4 ;
      font_set = false;
    }

    switch (Chinese[0][Chinese[0][0]]) {
      case 1:
        display.setFont(Chinese_1);
        break;
      case 2:
        display.setFont(Chinese_2);
        break;
      case 3:
        display.setFont(Chinese_3);
        break;
      case 4:
        display.setFont(Chinese_4);
        break;
      case 5:
        display.setFont(Chinese_5);
        break;
      case 6:
        display.setFont(Chinese_6);
        break;
      case 7:
        display.setFont(Chinese_7);
        break;
      case 8:
        display.setFont(Chinese_8);
        break;
      case 9:
        display.setFont(Chinese_9);
        break;
      case 10:
        display.setFont(Chinese_10);
        break;
      case 11:
        display.setFont(Chinese_11);
        break;
      case 12:
        display.setFont(Chinese_12);
        break;
      case 13:
        display.setFont(Chinese_13);
        break;
      case 14:
        display.setFont(Chinese_14);
        break;
      case 15:
        display.setFont(Chinese_15);
        break;
      case 16:
        display.setFont(Chinese_16);
        break;
      case 17:
        display.setFont(Chinese_17);
        break;
      case 18:
        display.setFont(Chinese_18);
        break;
      case 19:
        display.setFont(Chinese_19);
        break;
      case 20:
        display.setFont(Chinese_20);
        break;
      case 21:
        display.setFont(Chinese_21);
        break;
      case 22:
        display.setFont(Chinese_22);
        break;
      case 23:
        display.setFont(Chinese_23);
        break;
      case 24:
        display.setFont(Chinese_24);
        break;
      case 25:
        display.setFont(Chinese_25);
        break;
      case 26:
        display.setFont(Chinese_26);
        break;
      case 27:
        display.setFont(Chinese_27);
        break;
      case 28:
        display.setFont(Chinese_28);
        break;
      case 29:
        display.setFont(Chinese_29);
        break;
      case 30:
        display.setFont(Chinese_30);
        break;
      case 31:
        display.setFont(Chinese_31);
        break;
      case 32:
        display.setFont(Chinese_32);
        break;
      case 33:
        display.setFont(Chinese_33);
        break;
      case 34:
        display.setFont(Chinese_34);
        break;
      case 35:
        display.setFont(Chinese_35);
        break;
      case 36:
        display.setFont(Chinese_36);
        break;
      case 37:
        display.setFont(Chinese_37);
        break;
      case 38:
        display.setFont(Chinese_38);
        break;
      default :
        display.setFont(Dialog_plain_12);
        font_set = true;
        break;
    }
    Serial.println(display_x);
    display.drawString(display_x + horizontal * 12 , display_y + vertical * 15 , String(char(Chinese[1][Chinese[0][0]])));
    horizontal++;
    if (display_x + (horizontal + 1) * 12 > 128) {
      horizontal = 0;
      vertical++;
    }
  }
}
void loop() {
  ESP.deepSleep(100000, WAKE_RF_DISABLED);
}
