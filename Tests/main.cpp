#include <iostream>
#include <list>
#include <map>
#include <cstdlib>
#include <string>
/*
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
*/
struct Vertex2D
{
  int position[2];

  void setPos(int x, int y) {
      position[0] = x;
      position[1] = y;
  }

};

struct ColorVertex2D: public Vertex2D
{
  int color[4];

  void setColor(int r, int g, int b, int a) {
      color[0] = r;
      color[1] = g;
      color[2] = b;
      color[3] = a;
  }

};

template <class VertexType>
class Sprite {

public:
    void setPos(int x, int y) {
        for(int i=0; i < 4; ++i)
            verts[i].setPos(x, y);
    }

    void setColor(int r, int g, int b, int a) {
        for(int i=0; i < 4; ++i)
            verts[i].setColor(r, g, b, a);
    }

    int * arr() {
        return (int*)verts;
    }

private:
    VertexType verts[4];

};

int main() {

    Sprite<Vertex2D> Spr2d;
    Sprite<ColorVertex2D> SprCol2d;

    Spr2d.setPos(1, 2);
    for (int i = 0; i < 4*2; ++i) {
        std::cout << Spr2d.arr()[i] << " ";
    }
    std::cout << "\n";

    SprCol2d.setPos(3, 4);
    SprCol2d.setColor(1, 2, 3, 4);
    for (int i = 0; i < 4*6; ++i) {
        std::cout << SprCol2d.arr()[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
