#include <iostream>
using namespace std;

class Printer
{
public:
    Printer() {
        cout << "Constructor" << endl;
    }

    ~Printer() {
        cout << "Destructor!" << endl;
    }

    void Printing() {
        cout << "Printing!" << endl;
    }
};

template <class T>
class Log_Ptr
{
public:
    // Normaali konstruktori
    Log_Ptr(T* ptr) {
        m_ptr = ptr;
        m_count = new int(1);
        cout << "Omistajuus siirtynyt " << m_ptr << " (count=" << *m_count << ")\n";
    }

    // Kopiokonstruktori
    Log_Ptr(const Log_Ptr& other) {
        m_ptr = other.m_ptr;
        m_count = other.m_count;
        ++(*m_count);
        cout << "Kopioitu osoitin " << m_ptr << " (count=" << *m_count << ")\n";
    }

    Log_Ptr& operator=(const Log_Ptr& other) {
        if (this != &other) {
            // Vähennetään vanhan viittauksen laskuria
            release();

            // Kopioidaan uuden tiedot
            m_ptr = other.m_ptr;
            m_count = other.m_count;
            ++(*m_count);
            cout << "Sijoitusoperaattori, osoitin " << m_ptr << " (count=" << *m_count << ")\n";
        }
        return *this;
    }
    ~Log_Ptr() {
        release();
    }

    T* operator->() { return m_ptr; }
    T& operator* () { return *m_ptr; }

private:
    void release() {
        if (m_count) {
            --(*m_count);
            cout << "Viittaus vapautettu " << m_ptr << " (count=" << *m_count << ")\n";
            if (*m_count == 0) {
                cout << "Olio tuhottu   " << m_ptr << "\n";
                delete m_ptr;
                delete m_count;
            }
        }
    }

    T* m_ptr;
    int* m_count;
};

int main() {
    Log_Ptr<Printer> p1(new Printer);
    {
        Log_Ptr<Printer> p2 = p1;   // kopiointi
        p2->Printing();
        Log_Ptr<Printer> p3(new Printer);
        p3 = p1; // sijoitus
        (*p3).Printing();
    } // p2 ja p3 vapautuvat -> olio ei vielä tuhoudu, koska p1 yhä elossa

    p1->Printing();

    return 0;
}
