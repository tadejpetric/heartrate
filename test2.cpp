#define upper 890
#define lower 840
#define zamik 10 //milisekund, zamik*100 = sekunda

byte maxx(byte arr[zamik*10]) {
    byte tempy = arr[0];
    for (int i = 0; i<zamik*10; ++i)
        if (arr[i]>tempy) tempy = arr[i];
    return tempy;
}

byte minn(byte arr[zamik*10]) {
    byte tempy = arr[0];
    for (int i = 0; i<zamik*10; ++i)
        if (arr[i]<tempy) tempy = arr[i];
    return tempy;
}

class Container {
    byte branje[zamik*100]; //shranjuje za 2 sec vrednosti
    short current;
    byte nivo1;
    byte nivo2;
public:
    Container();
    short ret_current() {return current;}
    void insert(short value);
    void set_nivo(unsigned short loss = 2, short iv = 3);
    byte ret_nivo1() {return nivo1;}
    byte ret_nivo2() {return nivo2;}
    byte ret_branje(short i) {return branje[i];}

} meritev;

Container::Container() {
    for (int i = 0; i < zamik*100; ++i)
        branje[i] = 0;
    current = 0;

    nivo1 = 0;
    nivo2 = (upper - lower) * 5;
}

void Container::insert(short value) {
    branje[current] = value;
    //Serial.println(branje[current]);
    if(current == zamik*100-1)
        set_nivo();
    current = (current+1)%(zamik*100);
}

void Container::set_nivo(unsigned short loss = 2, short iv = 3) { //iv is on how many intervals to divide the interval
    //byte high = math::max_loose(branje, zamik*100, loss);
    //byte low = math::min_loose(branje, zamik*100, loss);
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


short getheart (byte pin) {
    short heart=analogRead(pin);
    if (heart > upper || heart < lower) //if not valid range -> 0
        heart=0;
    else
        heart -= (upper + lower) / 2; //if valid range -> normalize
    //heart*=10;

    return heart;
}

void setup() {
    Serial.begin(115200);
}

void loop() {
    short heart = getheart(A1); //reads heartrate from A1
    meritev.insert(heart);
    Serial.println(meritev.ret_nivo1());
    Serial.println(meritev.ret_nivo2());
    //Serial.println(meritev.ret_branje(meritev.ret_current()-1));
    //Serial.println(heart);
    
    delay(zamik);

    
}
