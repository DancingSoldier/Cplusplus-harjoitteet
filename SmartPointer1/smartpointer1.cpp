#include <iostream>
using namespace std;

template <class T>
class Log_Ptr
{
public:
	Log_Ptr(T* ptr) {
		cout << "Omistajuus siirtynyt " << ptr << "\n";
		m_ptr = ptr;
	}
	~Log_Ptr() {
		cout << "Olio tuhottu   " << m_ptr << "\n";
		delete m_ptr;
	}

private:
	Log_Ptr(Log_Ptr&) = delete;
	Log_Ptr& operator=(Log_Ptr&) = delete;
	T* m_ptr;
};class Printer{public:	Printer() {		cout << "Constructor" << endl;	}	~Printer() {		cout << "Destructor!" << endl;	}	void Printing() {		cout << "Printing!" << endl;	}};int main() {	Log_Ptr<Printer> p(new Printer);	return 0;}