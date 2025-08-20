#include <iostream>
#include <atomic>
#include <mutex>
using namespace std;

class Printer {
public:
    Printer() { cout << "Constructor\n"; }
    ~Printer() { cout << "Destructor!\n"; }
    void Printing() { cout << "Printing!\n"; }
};

template <class T>

// Struct joka pit‰‰ hallitun olion ja osoittimien m‰‰r‰n yhdess‰
struct ControlBlock {
    T* ptr;
    atomic<int> count;
    ControlBlock(T* p) : ptr(p), count(1) {}
};

template <class T>
class Log_Ptr {
public:
    explicit Log_Ptr(T* p) : ctrl(new ControlBlock<T>(p)) {
        log("Omistajuus siirtynyt", ctrl->ptr, ctrl->count.load());
    }

    // Kopiokonstruktori
    Log_Ptr(const Log_Ptr& other) : ctrl(other.ctrl) {
        ctrl->count.fetch_add(1);
        log("Kopioitu osoitin", ctrl->ptr, ctrl->count.load());
    }

    // Sijoitusoperaattori
    Log_Ptr& operator=(const Log_Ptr& other) {
        if (this != &other) {
            release();
            ctrl = other.ctrl;
            ctrl->count.fetch_add(1);
            log("Sijoitusoperaattori", ctrl->ptr, ctrl->count.load());
        }
        return *this;
    }

    ~Log_Ptr() { release(); }

    T* operator->() const { return ctrl->ptr; }
    T& operator* () const { return *ctrl->ptr; }

private:
    ControlBlock<T>* ctrl;

    void release() {
        if (ctrl && ctrl->count.fetch_sub(1) == 1) {
            log("Olio tuhottu", ctrl->ptr, 0);
            delete ctrl->ptr;
            delete ctrl;
        }
        else if (ctrl) {
            log("Viittaus vapautettu", ctrl->ptr, ctrl->count.load());
        }
    }

    static void log(const char* msg, void* p, int c) {
        static mutex m;
        lock_guard<mutex> lk(m);
        cout << msg << " " << p << " (count=" << c << ")\n";
    }
};

int main() {
    Log_Ptr<Printer> p1(new Printer);
    {
        Log_Ptr<Printer> p2 = p1;
        Log_Ptr<Printer> p3(new Printer);
        p3 = p1;
        p2->Printing();
        (*p3).Printing();
    }
    p1->Printing();
}
