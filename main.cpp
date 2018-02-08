#define upper 890
#define lower 840
#define zamik 10 //milisekund, zamik*100 = sekunda

namespace math {
    struct ignore {
        ignore *next;
        short extreme;
        short index;

        ignore() { next = nullptr; }
        ignore(short value, short neki) { next = nullptr; extreme = value; index = neki}
        void add(short value, short indx);
        void shift(short value);
        bool index_in(short indx);
        short read_last();
        void remove();
    } *start;

    void ignore::add(short value, short indx) {
        ignore *temp = *start;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = new ignore(value, indx);
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

    void ignore::shift(short value, short indx) { //shifts all values to the right. Inserts value at the start, deletes last value
        if(!start) return;
        ignore *temp = start;
        short prev = start->extreme;
        short prevx = start->index;
        start->extreme = value;
        start->index = indx;
        short next;
        short nextx;

        while(temp->next != 0) {
            temp = temp->next;

            next = temp->extreme;
            nextx = temp->index;

            temp->extreme = prev;
            temp->index = prevx;

            next = prev;
            nextx = prevx;
        }
        temp->extreme = next;
        temp->index = nextx;
    }

    bool ignore::index_in(short indx) {
        ignore *temp = start;
        while (!temp) {
            if(temp->index == indx) return true;
            temp = temp->next;
        }
        return false;
    }

    short ignore::read_last() {
        if(!start) return 0;
        ignore *temp = start;
        while(temp->next != nullptr) {
            temp = temp->next;
        }
        return temp->extreme;
    }

    short max_loose(short *tab, short size, unsigned short loss) { //returns <loss>th highest value. ex: loss = 2 => returns 2nd highest value
        start->remove();
        if(!loss) {
            Serial.println("loss cannot be 0");
            return 0;
        }

        for (int i = 0; i<loss; ++i) { //create a linked list of size loss
            start->add(0, 0);
        }

        short temp = tab[0];
        short index = 0;

        for (unsigned int j = 0; j<loss; ++j) {
            for (int i = 0; i<size; ++i) {
                if (tab[i]>temp && !start->index_in(i)) {
                    temp = tab[i];
                    index = i;
                }
            }
            start->shift(temp, index);
            }
        }

        temp = start->read_last();
        start->remove();
        return temp;
    }

    short min_loose(short *tab, unsigned short size, unsigned short loss) { //see: max_loose(). Implementation for minimum
        start->remove();
        if(!loss) {
            Serial.println("loss cannot be 0");
            return 0;
        }

        for (int i = 0; i<loss; ++i) { //create a linked list of size loss
            start->add(0, 0);
        }

        short temp = tab[0];
        short index = 0;

        for (unsigned int j = 0; j<loss; ++j) {
            for (int i = 0; i<size; ++i) {
                if (tab[i]<temp && !start->index_in(i)) {
                    temp = tab[i];
                    index = i;
                }
            }
            start->shift(temp, index);
            }
        }

        temp = start->read_last();
        start->remove();
        return temp;
    }
}

class Container {
    short branje[zamik*200]; //shranjuje za 2 sec vrednosti
    short current;
    short nivo1;
    short nivo2;
public:
    Container();
    void insert(short value);
    void set_nivo(unsigned short loss = 2, short iv = 3);
    short ret_nivo1() {return nivo1;}
    short ret_nivo2() {return nivo2;}
} meritev;

Container::Container() {
    for (int i = 0; i < zamik*200; ++i)
        branje[i] = 0;
    current = 0;

    nivo1 = 0;
    nivo2 = (upper - lower) * 5;
}

void Container::insert(short value) {
    branje[current] = value;
    current = (current+1)%(zamik*200);
}

void Container::set_nivo(unsigned short loss = 2, short iv = 3) { //iv is on how many intervals to divide the interval
    short high = math::max_loose(branje, zamik*200, loss);
    short low = math::min_loose(branje, zamik*200, loss);
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
