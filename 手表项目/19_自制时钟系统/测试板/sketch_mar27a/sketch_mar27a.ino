#define D6 13
#define D7 12
#define D8 11

#define second time[0]
#define minute time[1]
#define hour time[2]
#define day time[3]
#define week time[4]
#define month time[5]
#define year time[6]
//以下时间定义
unsigned long ms = millis();
unsigned long up_time = 0;
unsigned long pa_time = 0;
char time[7] = {0, 0, 0, 0, 0, 0, 0};
byte set = 0;
byte x = 91;
bool Check = false;
void setup() {
  Serial.begin(9600);
  pinMode(D6, INPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, INPUT);
}
void SEND() {
  pinMode(D6, OUTPUT);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, LOW);
  bool Send = false;
  up_time = millis();
  while (-1) {
    if ((digitalRead(D8) == LOW) && (x != 0)) {
      digitalWrite(D6, HIGH);
      digitalWrite(D7, HIGH);
    }
    if ((digitalRead(D8) == HIGH) && (x != 0)) {
      up_time = millis();
      while (x != 0) {
        if (millis() - up_time > 10) {
          break;
        }
        if (digitalRead(D8) == HIGH && !Send) {
          up_time = millis();
          if ((x & 0x01) == 0)
            digitalWrite(D7, LOW);
          else
            digitalWrite(D6, LOW);
          Send = true;
        }
        if (digitalRead(D8) == LOW && Send) {
          up_time = millis();
          Send = false;
          x >>= 1;
          digitalWrite(D6, HIGH);
          digitalWrite(D7, HIGH);
        }
      }
    }
    if (x == 0 || millis() - up_time > 10) {
      digitalWrite(D7, LOW);
      digitalWrite(D6, LOW);
      pinMode(D6, INPUT);
      break;
    }
  }
}

void TIME() {
  if (millis() - ms > 1000) {
    ms = millis();
    up_time = ms / 1000;
    ms = up_time * 1000;
    //    Serial.println(up_time);
    second = second + (up_time - pa_time);
    pa_time = up_time;
    for (byte i = 0; i < 2; i++) {
      while ((i == 0 || i == 1 || i == 2 ) && time[i] > 60) {
        time[i] -= 60;
        time[i + 1] += 1;
        if (i == 2) {
          time[i + 2] += 1;
        }
      }
    }
    if (time[4] == 7) {
      time[4] = 0;
    }
    if (((month == 1 || 3 || 5 || 7 || 8 || 10 || 12) && day == 32) || ((month == 4 || 6 || 9 || 11) && day == 31) || ((month == 2 && day == 30 && year % 4 == 0 && year % 100 != 0) || (month == 2 && day == 30 && year % 400 == 0)) || ((month == 2 && day == 30 && year % 4 != 0 && year % 100 == 0) || (month == 2 && day == 30 && year % 400 != 0))) {
      day = 1;
      month++;
    }
    if (month == 13) {
      month = 1;
      year ++;
    }
//    while (second >= 60) {
//      second -= 60;
//      minute ++;
//      while (minute >= 60) {
//        minute -= 60;
//        hour ++;
//        while (hour >= 24) {
//          hour -= 24;
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
    Serial.println(String(int(hour)) + String(':') + String(int(minute)) + String(':') + String(int(second)));
  }
}

int RECEIVE() {
  digitalWrite(D7, LOW);
  byte i = 0;
  byte R = 1;
  bool Receive = false;
  up_time = millis();
  while (-1) {
    //接收数据
    if (digitalRead(D6) == HIGH && digitalRead(D8) == HIGH && !Receive) {
      digitalWrite(D7, HIGH);
      Receive = true;
      up_time = millis();
    }
    if (digitalRead(D6) == HIGH && digitalRead(D8) == LOW && Receive) {
      R <<= 1;
      digitalWrite(D7, LOW);
      Receive = false;
      up_time = millis();
    }
    else if (digitalRead(D6) == LOW && digitalRead(D8) == HIGH && Receive) {
      i = i | R;
      R <<= 1;
      digitalWrite(D7, LOW);
      Receive = false;
      up_time = millis();
    }
    //检测终止信号
    if (digitalRead(D6) == LOW && digitalRead(D8) == LOW) {
      digitalWrite(D7, LOW);
      Receive = false;
      return i;
    }
    if (millis() - up_time > 10) {
      digitalWrite(D7, LOW);
      Receive = false;
      return 0;
    }
  }
}
void loop() {
  TIME();
  //模式一 : 检测起始信号
  if (digitalRead(D6) == HIGH && digitalRead(D8) == HIGH) {
    Serial.println("收到命令");
    digitalWrite(D7, HIGH);
    Check = true;
  }
  //进入哪个模式
  //1:接收模式
  if (digitalRead(D8) == HIGH && digitalRead(D6) == LOW && Check) {
    byte Receive = RECEIVE();
    Serial.println(Receive);
    if (Receive <= 7 && set == 0) {
      x = time[Receive - 1];
    }
    else if (Receive >= 8 && Receive <= 14 && set == 0) {
      set = Receive - 7;
      Receive = 0;
      up_time = millis();
    }
    if ((set != 0 && Receive != 0) || (set != 0 && (millis() - up_time > 20))) {
      time[set - 1] = Receive;
      Serial.println("设置" + String(set - 1) + "为" + String(time[set - 1]));
      set = 0;
    }
    Check = false;
  }
  //2.发送模式
  else if (digitalRead(D8) == LOW && digitalRead(D6) == HIGH && Check) {
    SEND();
    Check = false;
  }
}
