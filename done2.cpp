#include <SoftwareSerial.h>
#define LED 13
#define threshold 886
#define arsize 20
int heart;
bool cur;
SoftwareSerial BTserial(4, 7);

struct timr {
  unsigned long start;
  unsigned long over;
} inst;


struct bar {
  unsigned long arr[arsize];
  byte cur_bar;

  bar() {
    for (byte i = 0; i<arsize; ++i)
      arr[i] = 0;
      cur_bar = 0;
  }
  void insert(unsigned long val)
  {
    arr[cur_bar] = val;
    cur_bar = cur_bar+1;
    cur_bar = cur_bar%arsize;

  }

  float give() {
    float avg = 0;
    byte cnt = 0;
    for (byte i = 0; i<cur_bar - 1; ++i) {
      avg += arr[i+1]-arr[i];
      ++cnt;
    }
    avg = avg/cnt;
    return 60000/avg;
  }
  
} th;


void setup() {
  pinMode(LED,OUTPUT);
  Serial.begin(9600);
  BTserial.begin(9600);
}


void loop() {

  heart = analogRead(A0);
   //Serial.println(heart);


   if(!cur && heart > threshold){
     inst.start = millis();
     //Serial.println(true);
     cur = true;
   } else if (cur && heart < threshold) {
     //Serial.println(false);
     inst.over = millis();
     cur = false;
     th.insert((inst.start + inst.over)/2);
     if(th.cur_bar != 0 && th.cur_bar != 1) 
      {
        Serial.println(th.give()); 
        BTserial.println(th.give());
      }
     }
   

  digitalWrite(LED, cur);
  delay(10);
}
//April 3, 2018
