#define upper 890
#define lower 840
#define zamik 5 //milisekund, zamik*10 = sekunda
#define ROW 2
#define mult 100

#include <SoftwareSerial.h>
SoftwareSerial BTserial(8, 9); // RX, TX

int bpmcount(byte arr[mult*zamik], byte nivo1, byte nivo2, unsigned long cas) { //steje utripe iz arraya

    int cur = 0;
    bool pos = false;
    int count = 0;
    bool test;
    
    while (cur < mult*zamik-ROW) {
        if(pos) {
            test = false;
            for (int i = cur; i<cur+ROW; ++i) {
                if (arr[i]>nivo2)
                    test = true;
            }
        }
        else {
            test = true;
            for (int i = cur; i<cur+ROW; ++i) {
                if (arr[i]<nivo1)
                    test = false;
            }
        }
        if(test != pos) {
            ++count;
            pos = test;
        }
        ++cur;
    }
    return count*(60.0/((millis()-cas)/1000.0)/2);
}

byte maxx(byte arr[zamik*mult]) {       //najvisje vrednosti v arrayu
    byte tempy = arr[0];
    for (int i = 0; i<zamik*mult; ++i)
        if (arr[i]>tempy) tempy = arr[i];
    return tempy;
}

byte minn(byte arr[zamik*mult]) {       //najnizje vrednosti v arrayu
    byte tempy = arr[0];
    for (int i = 0; i<zamik*mult; ++i)
        if (arr[i]<tempy) tempy = arr[i];
    return tempy;
}

class Container {       //class s vsemi funkcijami & spremenljivkami
    byte branje[zamik*mult]; //shranjuje za 2 sec vrednosti
    short current;
    byte nivo1;
    byte nivo2;
    unsigned long cas;
    int bpm;
public:
    Container();
    short ret_current() {return current;}
    int insert(short value);
    void set_nivo(unsigned short loss = 2, short iv = 3);
    byte ret_nivo1() {return nivo1;}
    byte ret_nivo2() {return nivo2;}
    byte ret_branje(short i) {return branje[i];}

} meritev;

Container::Container() {        //nastavljanje začetnih vrednosti
    for (int i = 0; i < zamik*mult; ++i)
        branje[i] = 0;
    current = 0;

    nivo1 = 0;
    nivo2 = (upper - lower) * 5;
}

int Container::insert(short value) {        //rekalibracija nivojev intervala
    branje[current] = value;
    //Serial.println(branje[current]);
    if(current == zamik*mult-1)
        {
        set_nivo();
        //bpm = bpmcount(branje, nivo1, nivo2, cas);
        Serial.println(bpmcount(branje, nivo1, nivo2, cas));
        BTserial.print(bpmcount(branje, nivo1, nivo2, cas));
        cas = millis();
        }
    current = (current+1)%(zamik*mult);
    return bpm;
}

void Container::set_nivo(unsigned short loss = 2, short iv = 3) { //naredi tretjine iz arraya
    //byte high = math::max_loose(branje, zamik*mult, loss);
    //byte low = math::min_loose(branje, zamik*mult, loss);
    byte high = maxx(branje);
    byte low = minn(branje);
    //Serial.println(high);
    //Serial.println(low);
    short dist;
    if(high > 0) {
        if(low < 0)
            dist = (high-low)/iv;
        else
            dist = (high+low)/iv;
    }
    else {
        if(low < 0)
            dist = (-high-low)/iv;
        else
            dist = (-high+low)/iv;
    }
    nivo2 = high - dist;
    nivo1 = low + dist;
}


/*int Container::bpm() {
     return bpmcount(branje, nivo1, nivo2);
}*/

short getheart (byte pin) {         //bere vrednost senzorja in filtera nerelavantne podatke (spikes ipd)
    short heart=analogRead(pin);
    if (heart > upper || heart < lower) //if not valid range -> 0
        heart=0;
    else
        heart -= (upper + lower) / 2; //if valid range -> normalize
    //heart*=mult;

    return heart;
}

void setup() {
    Serial.begin(9600);
    BTserial.begin(9600);  
    
}

void loop() {
    short heart = getheart(A1); //reads heartrate from A1
    meritev.insert(heart);
    //Serial.println(meritev.ret_nivo1());
    //Serial.println(meritev.ret_nivo2());
    //Serial.println(meritev.ret_branje(meritev.ret_current()-1));
    //Serial.println(heart);
    //Serial.println(int(meritev.bpm()*60.0/15.0));
    //Serial.println(meritev.bpm());
    //Serial.println(heart);
    //BTserial.println(heart);
    delay(zamik);
}
//@ŽSM
