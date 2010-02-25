#include <iostream>
#include <list>
#include <map>
#include <cstdlib>
#include <string>

using namespace std;

class PortInterface {
    
    typedef std::list<PortInterface *>::iterator lit;
public:

    PortInterface(std::string n="name") : name(n) {};

    void showListeners() {
        for (lit it = listeners.begin(); it != listeners.end(); ++it) {
            std::cout << (*it)->name << std::endl;
        }
    }

    static void connect(PortInterface * p1, PortInterface * p2) {
        p1->listeners.push_back(p2);
    }

    template <class T>
    void set(const T & t) {
        internalSet( (void*)(&t) );
    }

protected:
    virtual void internalSet(void * ptr) = 0;

    template <class T> void internalWrite(const T & t) {
        std::cout << "Szukam odbiorców\n";
        for (lit it = listeners.begin(); it != listeners.end(); ++it) {
            //std::cout << "++internalWrite: " << t << std::endl;
            (*it)->set(t);
        }
    }

private:
    std::list<PortInterface *> listeners;
    std::string name;
};

template <class T>
class Port : public PortInterface {
public:
    Port(std::string n = "name") : PortInterface(n) {};

    void write (const T & t) {
        internalWrite(t);
    }

    T read() {
        if (buffer.size()>0) {
            T t = buffer.front();
            buffer.pop_front();
            return t;
        } else {
            throw "Buffer empty!";
        }
    }

protected:
    virtual void internalSet(void * ptr) {
        T t = *((T*)ptr);
        buffer.push_back(t);
        //std::cout << "++ Insert: " << t << std::endl;
    }

private:
    std::list<T> buffer;
};

class ConnectionManager {
public:
    void connect(PortInterface * p1, PortInterface * p2) {

    }

private:
    std::map<PortInterface *, PortInterface *> cons;
};

class Task1 {
public:
    Task1() : p1("t1::p1"), p2("t1::p2") {};

    void showAllP2() {
        std::cout << "Zawartoœæ Task1::p2\n";
        try {
            while (1)
                std::cout << p2.read() << std::endl;
        } catch(...) {
            std::cout << "==\n";
        }
    }

    void work() {
        int i = rand();
        p1.write(i);
        std::cout << "Task1::p1 <- " << i << std::endl;
    }

    Port<int> p1;
    Port<float> p2;
};

class Task2 {
public:
    Task2() : p1("t2::p1"), p2("t2::p2") {};

    void showAllP1() {
        std::cout << "Zawartoœæ Task2::p1\n";
        try {
            while(1)
                std::cout << p1.read() << std::endl;
        } catch(...) {
            std::cout << "==\n";
        }
    }

    void work() {
        float f = 0.123 * rand();
        p2.write(f);
        std::cout << "Task2::p2 <- " << f << std::endl;
    }

    void conn(Task1 * t) {
        PortInterface::connect(&p2, &(t->p2));
        PortInterface::connect(&(t->p1), &p1);
        std::cout << "listeners:\n";
        p2.showListeners();
        t->p1.showListeners();
    }

private:
    Port<bool> p1;
    Port<float> p2;
};

int main()
{
    Task1 t1;
    Task2 t2;
    t2.conn(&t1);

    for(int i = 1; i < 20; ++i) {
        if (rand()%2) {
            t1.showAllP2();
            t1.work();
        } else {
            t2.showAllP1();
            t2.work();
        }
    }

    cout << "Hello world!" << endl;
    return 0;
}

