#define upper 890
#define lower 840
#define zamik 10 //milisekund, zamik*100 = sekunda

namespace math {
    struct ignore {
        ignore *next;
        short extreme;

        ignore() { next = nullptr; }
        ignore(short value) { next = nullptr; extreme = value; }
        void add(short value);
        void write(short value);
        void remove();
    } *start;

    void ignore::add(short value) {
        ignore *temp = *start;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new ignore(value);
    }

    void ignore::remove(){
        if(start != nullptr) {
            while (start->next != nullptr) {
                temp = start->next;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                delete temp;
            }
        delete start;
        }
    }

    void ignore::write(short value) {
        ignore *temp = start;
        int vrednost = temp->extreme;
        while (temp->next != nullptr) {
            temp->extreme;
        }
    }

    short max_loose(short *tab, short size, unsigned short loss) { //returns <loss>th highest value. ex: loss = 2 => returns 2nd highest value
        start->remove();
        if(!loss) {
            Serial.println("loss cannot be 0");
            return 0;
        }
        short temp = tab[0];

        for (int i = 0; i<size; ++i) {
            if (tab[i]>temp) temp = tab[i];
        }

        start->remove();
        return temp;
    }

    short min_loose(short *tab, unsigned short size, unsigned short loss) { //see: max_loose(). Implementation for minimum
        if(!loss) {
            Serial.println("loss cannot be 0");
            return lower;
        }
        short temp = tab[0];
        for (int i = 0; i<size; ++i) {
            if(tab[i]<temp) temp = tab[i];
        }

        start->remove();
        return temp;
    }
}

class Container {
    short branje[zamik*200]; //shranjuje za 2 sec vrednosti
    unsigned short current;
    short nivo1;
    short nivo2;
public:
    Container();
    void set_nivo();
} meritev;

Container::Container() {
    for (int i = 0; i < zamik*200; ++i)
        branje[i] = 0;
    current = 0;

    nivo1 = 0;
    nivo2 = (upper - lower) * 5;
}

void Container::set_nivo() {

}


int getheart (byte pin) {
    int heart=analogRead(pin);
    if (heart > upper || heart < lower)
        heart=0;
    else
        heart -= (upper + lower) / 2;
    heart*=10;

    return heart;
}

void setup() {
    Serial.begin(115200);
    

}

void loop() {

    Serial.println(heart);
    delay(zamik);
}
