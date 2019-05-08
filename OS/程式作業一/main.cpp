#include <iostream>
#include "module1.h"
using namespace std;

int main() {
    greeting();
    cout << "In main, a = " << a << endl;
    a = 0;
    cout << "In main, a = " << a << endl;
	system("pause"); 
}
